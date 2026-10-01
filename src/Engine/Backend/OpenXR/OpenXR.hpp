#pragma once
#include "Graphics.hpp"

namespace IzEngine
{
	// An eye as the runtime located it, in the tracking space the frame is submitted in.
	struct XRView
	{
		XrPosef Pose{ { 0, 0, 0, 1 }, { 0, 0, 0 } };
		XrFovf Fov{};
	};

	struct XRSwapchain
	{
		XrSwapchain Handle = XR_NULL_HANDLE;
		std::vector<void*> Images;
		int Width = 0;
		int Height = 0;
	};

	// What a frame hands back to the compositor: the eyes it drew, and a flat panel held in front of the head,
	// or standing in the tracking space at PanelPose. An opaque panel ignores its alpha.
	struct XRLayers
	{
		bool Eyes = false;
		XRView Views[2];
		bool Panel = false;
		float PanelDistance = 1.5f;
		vec2 PanelSize{ 1.0f, 1.0f };
		std::optional<XrPosef> PanelPose;
		bool PanelAlpha = true;
	};

	// A binding the interaction profile does not have fails the whole suggestion, so each profile only
	// lists the inputs it really carries.
	struct XRBinding
	{
		XrAction Action = XR_NULL_HANDLE;
		const char* Path = nullptr;
	};

	// A stereo headset driven through OpenXR: the runtime, one session with its swapchains, the frame
	// loop, and the actions an application binds the controllers to. The graphics API it draws with is
	// left to an XRGraphics.
	class API OpenXR
	{
	public:
		static inline XRSwapchain EyeSwapchains[2];
		static inline XRSwapchain PanelSwapchain;
		static inline bool Swizzle = false;
		static inline bool PreferSteamVR = false;
		static inline bool WaitForHeadset = false;
		static inline std::function<void(XrSessionState)> OnStateChanged;
		static inline std::function<void()> OnRecentered;

		static bool Initialize(XRGraphics& graphics, std::string& error);
		static void Shutdown();
		static bool CreateSession(const glm::ivec2& eyeSize, const glm::ivec2& panelSize, std::string& error);
		static void DestroySession();

		static XrAction CreateAction(const char* name, const char* localized, XrActionType type);
		static void SuggestBindings(const char* profile, std::initializer_list<XRBinding> bindings);
		static bool GetBoolean(XrAction action);
		static float GetFloat(XrAction action);
		static vec2 GetVector2(XrAction action);
		static bool GetPose(XrAction action, XrPosef& pose);

		static void PollEvents();
		static bool BeginFrame();
		static void EndFrame(const XRLayers& layers);
		static void* Acquire(XRSwapchain& swapchain);
		static void Release(XRSwapchain& swapchain);

		static bool Ended();
		static bool Running();
		static bool Focused();
		static bool FrameOpen();
		static bool ShouldRender();
		static bool Located();
		static const XRView& View(int eye);
		static const XrPosef& Head();
		static glm::ivec2 RecommendedSize();
		static glm::ivec2 MaximumSize();
		static const std::string& RuntimeName();
		static const std::string& SystemName();
		static const char* StateName(XrSessionState state);
		static PFN_xrVoidFunction Function(const char* name);

	private:
		static inline XrInstance Instance = XR_NULL_HANDLE;
		static inline XrSystemId System = XR_NULL_SYSTEM_ID;
		static inline XrSession Session = XR_NULL_HANDLE;
		static inline XrSpace LocalSpace = XR_NULL_HANDLE;
		static inline XrSpace ViewSpace = XR_NULL_HANDLE;
		static inline XrSessionState State = XR_SESSION_STATE_UNKNOWN;
		static inline XrEnvironmentBlendMode BlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
		static inline XrViewConfigurationView Configurations[2] = {};
		static inline XRGraphics* Graphics = nullptr;
		static inline XrActionSet Actions = XR_NULL_HANDLE;
		static inline std::vector<std::pair<XrAction, XrSpace>> PoseSpaces;
		static inline std::string Runtime;
		static inline std::string Headset;
		static inline bool Redirected = false;

		static inline bool Started = false;
		static inline bool Exiting = false;
		static inline bool Open = false;
		static inline bool Synced = false;
		static inline XrFrameState Frame{ XR_TYPE_FRAME_STATE };
		static inline XRView Views[2];
		static inline XrPosef HeadPose{ { 0, 0, 0, 1 }, { 0, 0, 0 } };
		static inline bool HasPose = false;

		static bool SelectRuntime(std::string& error);
		static bool CreateInstance(XRGraphics& graphics, std::string& error);
		static bool CreateSwapchain(XRSwapchain& swapchain, int64_t format, const glm::ivec2& size);
		static void DestroySwapchain(XRSwapchain& swapchain);
		static void Locate();
		static void SyncActions();
		static XrPath Path(const char* path);
		static bool Check(XrResult result, const char* what);
	};
}
