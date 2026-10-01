#include "Vulkan.hpp"
#include "OpenXR.hpp"

namespace IzEngine
{
	namespace
	{
		// Renderers write gamma encoded bytes, which an sRGB image takes as they are.
		constexpr XRFormat Preferred[] = {
			{ VK_FORMAT_B8G8R8A8_SRGB, false },
			{ VK_FORMAT_R8G8B8A8_SRGB, true },
		};

		// Space separated, which is how OpenXR spells the lists.
		std::vector<std::string> Query(PFN_xrGetVulkanInstanceExtensionsKHR function, XrInstance instance,
			XrSystemId system)
		{
			uint32_t size = 0;
			if (XR_FAILED(function(instance, system, 0, &size, nullptr)) || !size)
				return {};

			std::string list(size, '\0');
			if (XR_FAILED(function(instance, system, size, &size, list.data())))
				return {};

			std::vector<std::string> names;
			std::istringstream stream(list.c_str());
			for (std::string name; stream >> name;)
				names.push_back(name);
			return names;
		}
	}

	const char* XRVulkan::Extension() const
	{
		return XR_KHR_VULKAN_ENABLE_EXTENSION_NAME;
	}

	// The requirements have to be asked for before a session, even by an application whose device already
	// exists. The extension lists are what that device and its instance must have been made with.
	bool XRVulkan::Initialize(XrInstance instance, XrSystemId system, std::string& error)
	{
		const auto requirementsFor = reinterpret_cast<PFN_xrGetVulkanGraphicsRequirementsKHR>(
			OpenXR::Function("xrGetVulkanGraphicsRequirementsKHR"));
		const auto instanceExtensions = reinterpret_cast<PFN_xrGetVulkanInstanceExtensionsKHR>(
			OpenXR::Function("xrGetVulkanInstanceExtensionsKHR"));
		const auto deviceExtensions = reinterpret_cast<PFN_xrGetVulkanDeviceExtensionsKHR>(
			OpenXR::Function("xrGetVulkanDeviceExtensionsKHR"));

		XrGraphicsRequirementsVulkanKHR requirements{ XR_TYPE_GRAPHICS_REQUIREMENTS_VULKAN_KHR };
		if (!requirementsFor || !instanceExtensions || !deviceExtensions
			|| XR_FAILED(requirementsFor(instance, system, &requirements)))
		{
			error = "the runtime gave no Vulkan requirements";
			return false;
		}

		Instance = instance;
		System = system;
		InstanceExtensions = Query(instanceExtensions, instance, system);
		DeviceExtensions = Query(deviceExtensions, instance, system);
		return true;
	}

	void XRVulkan::Shutdown()
	{
		Detach();
		InstanceExtensions.clear();
		DeviceExtensions.clear();
		Instance = XR_NULL_HANDLE;
		System = XR_NULL_SYSTEM_ID;
	}

	// The runtime composites on one GPU and only runs a session on a device made on that same one.
	bool XRVulkan::Attach(const XRVulkanDevice& device, std::string& error)
	{
		const auto deviceFor =
			reinterpret_cast<PFN_xrGetVulkanGraphicsDeviceKHR>(OpenXR::Function("xrGetVulkanGraphicsDeviceKHR"));

		VkPhysicalDevice physical = VK_NULL_HANDLE;
		if (!Instance || !deviceFor || XR_FAILED(deviceFor(Instance, System, device.Instance, &physical)))
		{
			error = "the runtime named no Vulkan GPU";
			return false;
		}
		if (physical != device.Physical)
		{
			error = "the headset is driven by another GPU than the game";
			return false;
		}

		Device = device;
		SessionBinding.instance = device.Instance;
		SessionBinding.physicalDevice = device.Physical;
		SessionBinding.device = device.Device;
		SessionBinding.queueFamilyIndex = device.QueueFamily;
		SessionBinding.queueIndex = device.QueueIndex;
		return true;
	}

	void XRVulkan::Detach()
	{
		Device = {};
		SessionBinding = { XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR };
	}

	const void* XRVulkan::Binding() const
	{
		return &SessionBinding;
	}

	std::span<const XRFormat> XRVulkan::Formats() const
	{
		return Preferred;
	}

	bool XRVulkan::Images(XrSwapchain swapchain, std::vector<uint64_t>& images)
	{
		uint32_t count = 0;
		xrEnumerateSwapchainImages(swapchain, 0, &count, nullptr);
		std::vector<XrSwapchainImageVulkanKHR> list(count, { XR_TYPE_SWAPCHAIN_IMAGE_VULKAN_KHR });
		if (XR_FAILED(xrEnumerateSwapchainImages(swapchain, count, &count,
				reinterpret_cast<XrSwapchainImageBaseHeader*>(list.data()))))
			return false;

		images.clear();
		for (const auto& image : list)
			images.push_back(reinterpret_cast<uint64_t>(image.image));
		return !images.empty();
	}

	void XRVulkan::Lock()
	{
		if (Device.Lock)
			Device.Lock();
	}

	void XRVulkan::Unlock()
	{
		if (Device.Unlock)
			Device.Unlock();
	}
}
