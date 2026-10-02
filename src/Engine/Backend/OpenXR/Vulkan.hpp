#pragma once
#include "Platform.hpp"

namespace IzEngine
{
	// The Vulkan device a session runs on, and how to hold its queue for the runtime when another thread
	// submits to it too.
	struct XRVulkanDevice
	{
		VkInstance Instance = VK_NULL_HANDLE;
		VkPhysicalDevice Physical = VK_NULL_HANDLE;
		VkDevice Device = VK_NULL_HANDLE;
		uint32_t QueueFamily = 0;
		uint32_t QueueIndex = 0;
		std::function<void()> Lock;
		std::function<void()> Unlock;
	};

	// OpenXR through Vulkan, on a device the application made. That device and its instance need the
	// extensions the runtime names, which are known once the binding is initialized.
	class API XRVulkan : public XRGraphics
	{
	public:
		std::vector<std::string> InstanceExtensions;
		std::vector<std::string> DeviceExtensions;

		const char* Extension() const override;
		bool Initialize(XrInstance instance, XrSystemId system, std::string& error) override;
		void Shutdown() override;
		const void* Binding() const override;
		std::span<const XRFormat> Formats() const override;
		bool Images(XrSwapchain swapchain, std::vector<uint64_t>& images) override;
		void Lock() override;
		void Unlock() override;

		bool Attach(const XRVulkanDevice& device, std::string& error);
		void Detach();

	private:
		XrInstance Instance = XR_NULL_HANDLE;
		XrSystemId System = XR_NULL_SYSTEM_ID;
		XRVulkanDevice Device;
		XrGraphicsBindingVulkanKHR SessionBinding{ XR_TYPE_GRAPHICS_BINDING_VULKAN_KHR };
	};
}
