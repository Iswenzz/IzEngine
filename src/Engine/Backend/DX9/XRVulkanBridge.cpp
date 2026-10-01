#include "XRVulkanBridge.hpp"

namespace
{
	// What Meta's and SteamVR's runtimes ask of the Vulkan instance and device, plus what DXVK enables on its
	// own. DXVK is handed these whether a headset is found or not, so one connected after the game started
	// still finds them in DXVK's device.
	constexpr const char* KnownInstance[] = {
		"VK_KHR_surface",
		"VK_KHR_win32_surface",
		"VK_KHR_external_memory_capabilities",
		"VK_KHR_external_fence_capabilities",
		"VK_KHR_external_semaphore_capabilities",
		"VK_KHR_get_physical_device_properties2",
		"VK_NV_external_memory_capabilities",
	};
	constexpr const char* KnownDevice[] = {
		"VK_KHR_swapchain",
		"VK_KHR_external_memory",
		"VK_KHR_external_memory_win32",
		"VK_KHR_external_fence",
		"VK_KHR_external_fence_win32",
		"VK_KHR_external_semaphore",
		"VK_KHR_external_semaphore_win32",
		"VK_KHR_get_memory_requirements2",
		"VK_KHR_dedicated_allocation",
		"VK_KHR_win32_keyed_mutex",
		"VK_KHR_timeline_semaphore",
	};

	void Merge(std::vector<std::string>& into, const std::vector<std::string>& names)
	{
		for (const std::string& name : names)
		{
			if (!std::ranges::contains(into, name))
				into.push_back(name);
		}
	}

	bool Has(const std::vector<VkExtensionProperties>& available, const std::string& name)
	{
		return std::ranges::any_of(available, [&](const VkExtensionProperties& e) { return name == e.extensionName; });
	}

	// DXVK enables whatever its extension provider lists without asking the driver, and a single name the
	// driver lacks fails its whole Vulkan instance or device, and the game with it. Only what the loader and
	// every GPU support is passed on; a runtime that needs the rest is refused a session later.
	void KeepSupported(std::vector<std::string>& instance, std::vector<std::string>& device)
	{
		const HMODULE vulkan = LoadLibraryA("vulkan-1.dll");
		const auto proc =
			vulkan ? reinterpret_cast<PFN_vkGetInstanceProcAddr>(GetProcAddress(vulkan, "vkGetInstanceProcAddr")) : nullptr;
		if (!proc)
		{
			instance.clear();
			device.clear();
			return;
		}
		const auto enumerateInstance = reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(
			proc(nullptr, "vkEnumerateInstanceExtensionProperties"));
		const auto createInstance = reinterpret_cast<PFN_vkCreateInstance>(proc(nullptr, "vkCreateInstance"));

		uint32_t count = 0;
		enumerateInstance(nullptr, &count, nullptr);
		std::vector<VkExtensionProperties> available(count);
		enumerateInstance(nullptr, &count, available.data());
		std::erase_if(instance, [&](const std::string& name) { return !Has(available, name); });

		VkApplicationInfo application{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
		application.apiVersion = VK_API_VERSION_1_1;
		VkInstanceCreateInfo info{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
		info.pApplicationInfo = &application;

		VkInstance probe = VK_NULL_HANDLE;
		if (createInstance(&info, nullptr, &probe) != VK_SUCCESS)
		{
			device.clear();
			return;
		}
		const auto destroy = reinterpret_cast<PFN_vkDestroyInstance>(proc(probe, "vkDestroyInstance"));
		const auto enumerateGpus =
			reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(proc(probe, "vkEnumeratePhysicalDevices"));
		const auto enumerateDevice = reinterpret_cast<PFN_vkEnumerateDeviceExtensionProperties>(
			proc(probe, "vkEnumerateDeviceExtensionProperties"));

		uint32_t gpus = 0;
		enumerateGpus(probe, &gpus, nullptr);
		std::vector<VkPhysicalDevice> physical(gpus);
		enumerateGpus(probe, &gpus, physical.data());

		for (VkPhysicalDevice gpu : physical)
		{
			enumerateDevice(gpu, nullptr, &count, nullptr);
			available.assign(count, {});
			enumerateDevice(gpu, nullptr, &count, available.data());
			std::erase_if(device, [&](const std::string& name) { return !Has(available, name); });
		}
		destroy(probe, nullptr);
	}

	// DXVK's provider contract: space separated, NUL terminated, the size counted with the terminator, 0 for
	// success. An empty list is a failure: DXVK would read the terminator as an extension named "".
	int WriteExtensions(const std::vector<std::string>& names, uint32_t capacity, uint32_t* count, char* buffer)
	{
		if (names.empty() || !count)
			return -1;

		std::string list;
		for (const std::string& name : names)
			list += (list.empty() ? "" : " ") + name;

		const auto size = static_cast<uint32_t>(list.size() + 1);
		*count = size;
		if (!capacity)
			return 0;
		if (capacity < size || !buffer)
			return -1;

		std::memcpy(buffer, list.c_str(), size);
		return 0;
	}
}

namespace IzEngine
{
	DX9XRVulkanBridge::DX9XRVulkanBridge(XRVulkan& graphics) : Graphics(graphics)
	{
		GPUResource::RegisterResource(this);
	}

	DX9XRVulkanBridge::~DX9XRVulkanBridge()
	{
		GPUResource::UnregisterResource(this);
		Detach();
	}

	// Only DXVK answers the interop query; the system d3d9 has no Vulkan device to share. The binding gets
	// DXVK's device and its queue lock, so the session can be made next.
	bool DX9XRVulkanBridge::Attach(IDirect3DDevice9* device, std::string& error)
	{
		if (Interop && device == D3D9)
			return true;
		Detach();

		if (!device || FAILED(device->QueryInterface(__uuidof(ID3D9VkInteropDevice), reinterpret_cast<void**>(&Interop))))
		{
			Interop = nullptr;
			error = "VR needs the game to run on DXVK";
			return false;
		}
		D3D9 = device;

		XRVulkanDevice handles;
		Interop->GetVulkanHandles(&handles.Instance, &handles.Physical, &handles.Device);
		Interop->GetSubmissionQueue(&Queue, &handles.QueueIndex, &handles.QueueFamily);
		handles.Lock = [this] { Interop->LockSubmissionQueue(); };
		handles.Unlock = [this] { Interop->ReleaseSubmissionQueue(); };
		Device = handles.Device;
		QueueFamily = handles.QueueFamily;

		if (!CreateCommands())
		{
			error = LastError;
			Detach();
			return false;
		}
		if (!Graphics.Attach(handles, error))
		{
			Detach();
			return false;
		}
		return true;
	}

	// After the session that used DXVK's queue, and before DXVK's device goes.
	void DX9XRVulkanBridge::Detach()
	{
		Release();
		Wait();
		Graphics.Detach();

		for (DX9VulkanCopy& copy : Copies)
		{
			if (copy.Fence)
				Vk.DestroyFence(Device, copy.Fence, nullptr);
			copy = {};
		}
		if (Pool)
			Vk.DestroyCommandPool(Device, Pool, nullptr);
		Pool = VK_NULL_HANDLE;

		if (Interop)
			Interop->Release();
		Interop = nullptr;
		D3D9 = nullptr;
		Device = VK_NULL_HANDLE;
		Queue = VK_NULL_HANDLE;
	}

	bool DX9XRVulkanBridge::CreateCommands()
	{
		const HMODULE vulkan = GetModuleHandleA("vulkan-1.dll");
		Vk.GetDeviceProcAddr =
			vulkan ? reinterpret_cast<PFN_vkGetDeviceProcAddr>(GetProcAddress(vulkan, "vkGetDeviceProcAddr")) : nullptr;

		const auto resolve = [&](const char* name, auto& function)
		{
			function = reinterpret_cast<std::remove_reference_t<decltype(function)>>(Vk.GetDeviceProcAddr(Device, name));
			return function != nullptr;
		};
		const bool resolved = Vk.GetDeviceProcAddr && resolve("vkCreateCommandPool", Vk.CreateCommandPool)
			&& resolve("vkDestroyCommandPool", Vk.DestroyCommandPool)
			&& resolve("vkAllocateCommandBuffers", Vk.AllocateCommandBuffers)
			&& resolve("vkResetCommandBuffer", Vk.ResetCommandBuffer)
			&& resolve("vkBeginCommandBuffer", Vk.BeginCommandBuffer) && resolve("vkEndCommandBuffer", Vk.EndCommandBuffer)
			&& resolve("vkCmdPipelineBarrier", Vk.CmdPipelineBarrier) && resolve("vkCmdCopyImage", Vk.CmdCopyImage)
			&& resolve("vkQueueSubmit", Vk.QueueSubmit) && resolve("vkCreateFence", Vk.CreateFence)
			&& resolve("vkDestroyFence", Vk.DestroyFence) && resolve("vkWaitForFences", Vk.WaitForFences)
			&& resolve("vkResetFences", Vk.ResetFences);
		if (!resolved)
		{
			LastError = "Vulkan entry points are missing";
			return false;
		}

		VkCommandPoolCreateInfo pool{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
		pool.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		pool.queueFamilyIndex = QueueFamily;
		if (Vk.CreateCommandPool(Device, &pool, nullptr, &Pool) != VK_SUCCESS)
		{
			LastError = "vkCreateCommandPool failed";
			return false;
		}

		VkCommandBufferAllocateInfo allocate{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		allocate.commandPool = Pool;
		allocate.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocate.commandBufferCount = 1;

		VkFenceCreateInfo fence{ VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
		for (DX9VulkanCopy& copy : Copies)
		{
			if (Vk.AllocateCommandBuffers(Device, &allocate, &copy.Commands) != VK_SUCCESS
				|| Vk.CreateFence(Device, &fence, nullptr, &copy.Fence) != VK_SUCCESS)
			{
				LastError = "Vulkan command buffers could not be created";
				return false;
			}
		}
		return true;
	}

	bool DX9XRVulkanBridge::CaptureEye(int eye, IDirect3DSurface9* source)
	{
		return Capture(Targets[eye], OpenXR::EyeSwapchains[eye], source);
	}

	bool DX9XRVulkanBridge::CapturePanel(IDirect3DSurface9* source)
	{
		return Capture(Targets[2], OpenXR::PanelSwapchain, source);
	}

	// Copies the captures into the runtime's images for the layers asked for, and drops a layer whose copy
	// failed. An eye not captured this frame is copied again from its last capture.
	void DX9XRVulkanBridge::Submit(XRLayers& layers)
	{
		if (!Interop)
		{
			layers.Eyes = false;
			layers.Panel = false;
			return;
		}

		// What DXVK recorded goes to its submission thread first, so the captures reach the queue ahead of the
		// copies that read them.
		Interop->FlushRenderingCommands();

		if (layers.Eyes)
			layers.Eyes = Present(Targets[0], OpenXR::EyeSwapchains[0]) && Present(Targets[1], OpenXR::EyeSwapchains[1]);
		if (layers.Panel)
			layers.Panel = Present(Targets[2], OpenXR::PanelSwapchain);
	}

	const std::string& DX9XRVulkanBridge::Error() const
	{
		return LastError;
	}

	// The staging targets are D3DPOOL_DEFAULT, which the device has to be rid of before a reset.
	void DX9XRVulkanBridge::Release()
	{
		for (DX9VulkanTarget& target : Targets)
			ReleaseTarget(target);
	}

	void DX9XRVulkanBridge::ReleaseTarget(DX9VulkanTarget& target)
	{
		if (target.Staging)
		{
			Wait();
			target.Staging->Release();
		}
		target = {};
	}

	// A frame of another size than the swapchain is scaled to it.
	bool DX9XRVulkanBridge::Capture(DX9VulkanTarget& target, const XRSwapchain& swapchain, IDirect3DSurface9* source)
	{
		if (!D3D9 || !source || !swapchain.Handle)
			return false;
		if (target.Staging && (target.Width != swapchain.Width || target.Height != swapchain.Height))
			ReleaseTarget(target);
		if (!target.Staging && !CreateStaging(target, swapchain))
			return false;

		D3DSURFACE_DESC desc = {};
		source->GetDesc(&desc);
		const bool scaled = static_cast<int>(desc.Width) != target.Width || static_cast<int>(desc.Height) != target.Height;

		if (FAILED(D3D9->StretchRect(source, nullptr, target.Staging, nullptr, scaled ? D3DTEXF_LINEAR : D3DTEXF_NONE)))
			return false;
		target.Captured = true;
		return true;
	}

	// The copy is byte for byte, so the staging target holds the same four bytes per texel as the swapchain.
	bool DX9XRVulkanBridge::CreateStaging(DX9VulkanTarget& target, const XRSwapchain& swapchain)
	{
		const D3DFORMAT format = OpenXR::Swizzle ? D3DFMT_A8B8G8R8 : D3DFMT_A8R8G8B8;
		if (FAILED(D3D9->CreateRenderTarget(swapchain.Width, swapchain.Height, format, D3DMULTISAMPLE_NONE, 0, FALSE,
				&target.Staging, nullptr)))
		{
			target.Staging = nullptr;
			LastError = "the staging render target could not be created";
			return false;
		}

		ID3D9VkInteropTexture* texture = nullptr;
		VkImageCreateInfo info{ VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
		const bool mapped = SUCCEEDED(
								target.Staging->QueryInterface(__uuidof(ID3D9VkInteropTexture), reinterpret_cast<void**>(&texture)))
			&& SUCCEEDED(texture->GetVulkanImageInfo(&target.Image, &target.Layout, &info));
		if (texture)
			texture->Release();

		const VkFormat expected = OpenXR::Swizzle ? VK_FORMAT_R8G8B8A8_UNORM : VK_FORMAT_B8G8R8A8_UNORM;
		if (!mapped || info.format != expected || !(info.usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT))
		{
			ReleaseTarget(target);
			LastError = "DXVK did not expose the staging image";
			return false;
		}
		target.Width = swapchain.Width;
		target.Height = swapchain.Height;
		return true;
	}

	bool DX9XRVulkanBridge::Present(const DX9VulkanTarget& target, XRSwapchain& swapchain)
	{
		if (!target.Captured || !swapchain.Handle)
			return false;

		DX9VulkanCopy* copy = NextCopy();
		if (!copy)
			return false;

		const uint64_t image = OpenXR::Acquire(swapchain);
		if (!image)
			return false;

		Record(target, copy->Commands, reinterpret_cast<VkImage>(image));

		VkSubmitInfo submit{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
		submit.commandBufferCount = 1;
		submit.pCommandBuffers = &copy->Commands;

		Interop->LockSubmissionQueue();
		copy->Submitted = Vk.QueueSubmit(Queue, 1, &submit, copy->Fence) == VK_SUCCESS;
		Interop->ReleaseSubmissionQueue();

		OpenXR::Release(swapchain);
		return copy->Submitted;
	}

	// Staging goes from DXVK's resting layout to a copy source and back, the runtime's image from colour
	// attachment to copy destination and back, which is the layout OpenXR hands it over and takes it back in.
	void DX9XRVulkanBridge::Record(const DX9VulkanTarget& target, VkCommandBuffer commands, VkImage image) const
	{
		const VkImageSubresourceRange range = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

		VkCommandBufferBeginInfo begin{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
		begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		Vk.BeginCommandBuffer(commands, &begin);

		VkImageMemoryBarrier before[2] = { { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER },
			{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER } };
		before[0].srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
		before[0].dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		before[0].oldLayout = target.Layout;
		before[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		before[0].image = target.Image;
		before[1].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		before[1].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		before[1].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		before[1].image = image;
		for (VkImageMemoryBarrier& barrier : before)
		{
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.subresourceRange = range;
		}
		Vk.CmdPipelineBarrier(commands, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0,
			nullptr, 0, nullptr, 2, before);

		VkImageCopy region = {};
		region.srcSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
		region.dstSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
		region.extent = { static_cast<uint32_t>(target.Width), static_cast<uint32_t>(target.Height), 1 };
		Vk.CmdCopyImage(commands, target.Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		// ALL_COMMANDS as the destination orders DXVK's next write to the staging image after this read.
		VkImageMemoryBarrier after[2] = { before[0], before[1] };
		after[0].srcAccessMask = 0;
		after[0].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
		after[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		after[0].newLayout = target.Layout;
		after[1].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		after[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		after[1].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		after[1].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		Vk.CmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0,
			nullptr, 0, nullptr, 2, after);

		Vk.EndCommandBuffer(commands);
	}

	// The next command buffer in the ring, once the GPU is done with its last copy. Waited on outside the
	// queue lock, so DXVK keeps submitting meanwhile.
	DX9VulkanCopy* DX9XRVulkanBridge::NextCopy()
	{
		DX9VulkanCopy& copy = Copies[Next++ % Copies.size()];
		if (copy.Submitted)
		{
			if (Vk.WaitForFences(Device, 1, &copy.Fence, VK_TRUE, UINT64_MAX) != VK_SUCCESS)
				return nullptr;
			copy.Submitted = false;
		}
		Vk.ResetFences(Device, 1, &copy.Fence);
		Vk.ResetCommandBuffer(copy.Commands, 0);
		return &copy;
	}

	// For the copies still in flight, before an image they read or write goes away.
	void DX9XRVulkanBridge::Wait()
	{
		for (DX9VulkanCopy& copy : Copies)
		{
			if (copy.Submitted)
				Vk.WaitForFences(Device, 1, &copy.Fence, VK_TRUE, UINT64_MAX);
			copy.Submitted = false;
		}
	}

	// What DXVK is handed at startup: the known lists, and the runtime's own when a headset is already there.
	void DXVKExtensions::Prepare(const XRVulkan& graphics)
	{
		Instance.assign(std::begin(KnownInstance), std::end(KnownInstance));
		Device.assign(std::begin(KnownDevice), std::end(KnownDevice));
		Merge(Instance, graphics.InstanceExtensions);
		Merge(Device, graphics.DeviceExtensions);
		KeepSupported(Instance, Device);
	}

	// The runtime's Vulkan extensions DXVK was not given: a session on a device without them is left unmade.
	std::vector<std::string> DXVKExtensions::Missing(const XRVulkan& graphics)
	{
		std::vector<std::string> missing;
		for (const std::string& name : graphics.InstanceExtensions)
		{
			if (!std::ranges::contains(Instance, name))
				missing.push_back(name);
		}
		for (const std::string& name : graphics.DeviceExtensions)
		{
			if (!std::ranges::contains(Device, name))
				missing.push_back(name);
		}
		return missing;
	}

	// The module these exports live in. The handle holds a reference, which DXVK's FreeLibrary returns.
	HMODULE DXVKExtensions::Provider()
	{
		if (Instance.empty() && Device.empty())
			return nullptr;

		HMODULE module = nullptr;
		GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<LPCSTR>(&DXVKExtensions::Provider),
			&module);
		return module;
	}
}

extern "C" int WINAPI IzEngineDXVKInstanceExtensions(uint32_t capacity, uint32_t* count, char* buffer)
{
#pragma comment(linker, "/EXPORT:__wineopenxr_GetVulkanInstanceExtensions=" __FUNCDNAME__)
	return WriteExtensions(IzEngine::DXVKExtensions::Instance, capacity, count, buffer);
}

extern "C" int WINAPI IzEngineDXVKDeviceExtensions(uint32_t capacity, uint32_t* count, char* buffer)
{
#pragma comment(linker, "/EXPORT:__wineopenxr_GetVulkanDeviceExtensions=" __FUNCDNAME__)
	const int result = WriteExtensions(IzEngine::DXVKExtensions::Device, capacity, count, buffer);
	if (result == 0 && capacity)
		IzEngine::DXVKExtensions::Provided = true;
	return result;
}
