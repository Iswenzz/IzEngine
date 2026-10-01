#pragma once
#include "Base.hpp"
#include "DXVK.hpp"

#include "Engine/Backend/OpenXR/OpenXR.hpp"
#include "Engine/Renderer/Base/GPUResource.hpp"

namespace IzEngine
{
	// The Vulkan device entry points the copies use, from the loader DXVK runs on.
	struct DX9VulkanFunctions
	{
		PFN_vkGetDeviceProcAddr GetDeviceProcAddr = nullptr;
		PFN_vkCreateCommandPool CreateCommandPool = nullptr;
		PFN_vkDestroyCommandPool DestroyCommandPool = nullptr;
		PFN_vkAllocateCommandBuffers AllocateCommandBuffers = nullptr;
		PFN_vkResetCommandBuffer ResetCommandBuffer = nullptr;
		PFN_vkBeginCommandBuffer BeginCommandBuffer = nullptr;
		PFN_vkEndCommandBuffer EndCommandBuffer = nullptr;
		PFN_vkCmdPipelineBarrier CmdPipelineBarrier = nullptr;
		PFN_vkCmdCopyImage CmdCopyImage = nullptr;
		PFN_vkQueueSubmit QueueSubmit = nullptr;
		PFN_vkCreateFence CreateFence = nullptr;
		PFN_vkDestroyFence DestroyFence = nullptr;
		PFN_vkWaitForFences WaitForFences = nullptr;
		PFN_vkResetFences ResetFences = nullptr;
	};

	// One recorded copy, and the fence that says when its command buffer may be recorded again.
	struct DX9VulkanCopy
	{
		VkCommandBuffer Commands = VK_NULL_HANDLE;
		VkFence Fence = VK_NULL_HANDLE;
		bool Submitted = false;
	};

	// What a swapchain is fed from: a DXVK render target of its size, holding the last capture. Its VkImage is
	// asked for at every copy, as DXVK may move the image to other memory.
	struct DX9VulkanTarget
	{
		IDirect3DSurface9* Staging = nullptr;
		ID3D9VkInteropTexture* Texture = nullptr;
		int Width = 0;
		int Height = 0;
		bool Captured = false;
	};

	// Carries what a D3D9 renderer running on DXVK draws to OpenXR's Vulkan swapchains. The session runs on
	// DXVK's own Vulkan device, so a capture is copied on the GPU within the frame. DXVK submits from its own
	// thread, so its queue is held for every submission, the runtime's included.
	class API DX9XRVulkanBridge : public GPUResource
	{
	public:
		DX9XRVulkanBridge(XRVulkan& graphics);
		~DX9XRVulkanBridge() override;

		bool Attach(IDirect3DDevice9* device, std::string& error);
		void Detach();
		bool CaptureEye(int eye, IDirect3DSurface9* source);
		bool CapturePanel(IDirect3DSurface9* source);
		void Submit(XRLayers& layers);
		const std::string& Error() const;

		void Release() override;

	private:
		XRVulkan& Graphics;
		IDirect3DDevice9* D3D9 = nullptr;
		ID3D9VkInteropDevice* Interop = nullptr;
		DX9VulkanFunctions Vk;
		VkDevice Device = VK_NULL_HANDLE;
		VkQueue Queue = VK_NULL_HANDLE;
		uint32_t QueueFamily = 0;
		VkCommandPool Pool = VK_NULL_HANDLE;
		std::array<DX9VulkanCopy, 8> Copies;
		size_t Next = 0;
		DX9VulkanTarget Targets[3];
		std::string LastError;

		bool CreateCommands();
		bool Capture(DX9VulkanTarget& target, const XRSwapchain& swapchain, IDirect3DSurface9* source);
		bool CreateStaging(DX9VulkanTarget& target, const XRSwapchain& swapchain);
		void ReleaseTarget(DX9VulkanTarget& target);
		bool Present(const DX9VulkanTarget& target, XRSwapchain& swapchain);
		void Record(const DX9VulkanTarget& target, VkImage staging, VkImageLayout layout, VkCommandBuffer commands,
			VkImage image) const;
		DX9VulkanCopy* NextCopy();
		void Wait();
	};

	// The Vulkan extensions DXVK makes its instance and device with. DXVK asks for them from the OpenXR
	// provider it loads as wineopenxr.dll; a game answers that load with Provider(), the module that exports
	// the two functions it calls.
	class API DXVKExtensions
	{
	public:
		static inline std::vector<std::string> Instance;
		static inline std::vector<std::string> Device;
		static inline bool Provided = false;

		static void Prepare(const XRVulkan& graphics);
		static std::vector<std::string> Missing(const XRVulkan& graphics);
		static HMODULE Provider();
	};
}
