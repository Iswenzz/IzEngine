#include "OpenXR.hpp"

namespace IzEngine
{
	namespace
	{
		constexpr XrViewConfigurationType ViewConfiguration = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
		constexpr const char* RuntimeVariable = "XR_RUNTIME_JSON";

		// The manifest the loader would pick. A 32-bit process reads the WOW6432Node copy of the key,
		// which is the runtime registered for 32-bit applications.
		std::string ActiveRuntime()
		{
			char path[MAX_PATH] = {};
			DWORD size = sizeof(path);
			if (RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Khronos\\OpenXR\\1", "ActiveRuntime", RRF_RT_REG_SZ,
					nullptr, path, &size)
				!= ERROR_SUCCESS)
				return {};
			return path;
		}

		// SteamVR records where it is installed for every OpenVR client to find.
		std::filesystem::path SteamVRRuntime()
		{
			char* local = nullptr;
			size_t length = 0;
			if (_dupenv_s(&local, &length, "LOCALAPPDATA") || !local)
				return {};

			const std::filesystem::path registry = std::filesystem::path(local) / "openvr" / "openvrpaths.vrpath";
			free(local);

			std::ifstream file(registry);
			const nlohmann::json paths = nlohmann::json::parse(file, nullptr, false);
			if (!paths.is_object() || !paths.contains("runtime") || !paths["runtime"].is_array())
				return {};

			for (const auto& runtime : paths["runtime"])
			{
				if (!runtime.is_string())
					continue;

				std::error_code ec;
				const std::filesystem::path manifest = std::filesystem::path(runtime.get<std::string>()) / "steamxr_win32.json";
				if (std::filesystem::exists(manifest, ec))
					return manifest;
			}
			return {};
		}
	}

	// Meta's 32-bit runtime takes the process down inside xrCreateSession, before it can return an
	// error, so a Quest on Link goes through SteamVR's 32-bit runtime instead, which drives the same
	// headset over Link. A manifest named in XR_RUNTIME_JSON is left alone.
	bool OpenXR::SelectRuntime(std::string& error)
	{
#ifdef PLATFORM_32
		char named[MAX_PATH] = {};
		if (!Redirected && GetEnvironmentVariableA(RuntimeVariable, named, sizeof(named)))
			return true;

		std::string active = ActiveRuntime();
		std::ranges::transform(active, active.begin(), [](char c) { return static_cast<char>(std::tolower(c)); });
		if (!active.contains("oculus"))
		{
			if (Redirected)
				SetEnvironmentVariableA(RuntimeVariable, nullptr);
			Redirected = false;
			return true;
		}

		const std::filesystem::path steamvr = SteamVRRuntime();
		if (steamvr.empty())
		{
			error = "Meta's 32-bit OpenXR runtime cannot run this game, install SteamVR or use Virtual Desktop";
			return false;
		}
		Redirected = SetEnvironmentVariableA(RuntimeVariable, steamvr.string().c_str());
#endif
		return true;
	}

	bool OpenXR::Initialize(XRGraphics& graphics, std::string& error)
	{
		if (Instance)
			return true;
		if (!SelectRuntime(error))
			return false;

		uint32_t count = 0;
		xrEnumerateInstanceExtensionProperties(nullptr, 0, &count, nullptr);
		std::vector<XrExtensionProperties> extensions(count, { XR_TYPE_EXTENSION_PROPERTIES });
		xrEnumerateInstanceExtensionProperties(nullptr, count, &count, extensions.data());

		const char* binding = graphics.Extension();
		const bool supported = std::ranges::any_of(extensions,
			[&](const XrExtensionProperties& e) { return std::string_view(e.extensionName) == binding; });
		if (!supported)
		{
			error = count ? std::format("the OpenXR runtime has no {}", binding)
						  : "no OpenXR runtime is installed for this process (Meta Quest Link, SteamVR, Virtual Desktop...)";
			return false;
		}

		const char* enabled[] = { binding };

		XrInstanceCreateInfo info{ XR_TYPE_INSTANCE_CREATE_INFO };
		strcpy_s(info.applicationInfo.applicationName, APPLICATION_ID);
		strcpy_s(info.applicationInfo.engineName, "IzEngine");
		info.applicationInfo.applicationVersion = 1;
		info.applicationInfo.engineVersion = 1;
		info.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;
		info.enabledExtensionCount = 1;
		info.enabledExtensionNames = enabled;

		XrResult result = xrCreateInstance(&info, &Instance);
		if (XR_FAILED(result))
		{
			error = std::format("xrCreateInstance failed ({})", static_cast<int>(result));
			Instance = XR_NULL_HANDLE;
			return false;
		}

		XrInstanceProperties properties{ XR_TYPE_INSTANCE_PROPERTIES };
		if (XR_SUCCEEDED(xrGetInstanceProperties(Instance, &properties)))
			Runtime = properties.runtimeName;

		XrSystemGetInfo system{ XR_TYPE_SYSTEM_GET_INFO };
		system.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
		result = xrGetSystem(Instance, &system, &System);
		if (XR_FAILED(result))
		{
			error = result == XR_ERROR_FORM_FACTOR_UNAVAILABLE
				? std::format("{} sees no headset, connect it and start the link first", Runtime)
				: std::format("xrGetSystem failed ({})", static_cast<int>(result));
			Shutdown();
			return false;
		}

		count = 0;
		xrEnumerateViewConfigurationViews(Instance, System, ViewConfiguration, 0, &count, nullptr);
		if (count != 2)
		{
			error = std::format("{} does not drive a stereo headset", Runtime);
			Shutdown();
			return false;
		}
		Configurations[0] = Configurations[1] = { XR_TYPE_VIEW_CONFIGURATION_VIEW };
		xrEnumerateViewConfigurationViews(Instance, System, ViewConfiguration, count, &count, Configurations);

		count = 0;
		xrEnumerateEnvironmentBlendModes(Instance, System, ViewConfiguration, 0, &count, nullptr);
		std::vector<XrEnvironmentBlendMode> modes(count);
		xrEnumerateEnvironmentBlendModes(Instance, System, ViewConfiguration, count, &count, modes.data());
		BlendMode = modes.empty() || std::ranges::contains(modes, XR_ENVIRONMENT_BLEND_MODE_OPAQUE)
			? XR_ENVIRONMENT_BLEND_MODE_OPAQUE
			: modes.front();

		Graphics = &graphics;
		if (!graphics.Initialize(Instance, System, error))
		{
			Shutdown();
			return false;
		}
		return true;
	}

	void OpenXR::Shutdown()
	{
		DestroySession();

		if (Actions)
			xrDestroyActionSet(Actions);
		Actions = XR_NULL_HANDLE;
		PoseSpaces.clear();

		if (Graphics)
			Graphics->Shutdown();
		Graphics = nullptr;

		if (Instance)
			xrDestroyInstance(Instance);
		Instance = XR_NULL_HANDLE;
		System = XR_NULL_SYSTEM_ID;
		Exiting = false;
	}

	// The actions have to be made and their bindings suggested before this, since the session takes
	// its action set as it is and for good.
	bool OpenXR::CreateSession(const glm::ivec2& eyeSize, const glm::ivec2& panelSize, std::string& error)
	{
		if (!Instance || !Graphics || Session)
			return Session != XR_NULL_HANDLE;

		XrSessionCreateInfo info{ XR_TYPE_SESSION_CREATE_INFO };
		info.next = Graphics->Binding();
		info.systemId = System;

		XrResult result = xrCreateSession(Instance, &info, &Session);
		if (XR_FAILED(result))
		{
			error = std::format("xrCreateSession failed ({})", static_cast<int>(result));
			Session = XR_NULL_HANDLE;
			return false;
		}
		State = XR_SESSION_STATE_UNKNOWN;

		XrReferenceSpaceCreateInfo space{ XR_TYPE_REFERENCE_SPACE_CREATE_INFO };
		space.poseInReferenceSpace = { { 0, 0, 0, 1 }, { 0, 0, 0 } };
		space.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
		xrCreateReferenceSpace(Session, &space, &LocalSpace);
		space.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_VIEW;
		xrCreateReferenceSpace(Session, &space, &ViewSpace);

		uint32_t count = 0;
		xrEnumerateSwapchainFormats(Session, 0, &count, nullptr);
		std::vector<int64_t> formats(count);
		xrEnumerateSwapchainFormats(Session, count, &count, formats.data());

		int64_t format = 0;
		for (const XRFormat& candidate : Graphics->Formats())
		{
			if (!std::ranges::contains(formats, candidate.Format))
				continue;
			format = candidate.Format;
			Swizzle = candidate.Swizzle;
			break;
		}
		if (!format)
		{
			error = "the runtime offers no 8-bit colour swapchain";
			DestroySession();
			return false;
		}

		if (!CreateSwapchain(EyeSwapchains[0], format, eyeSize) || !CreateSwapchain(EyeSwapchains[1], format, eyeSize)
			|| !CreateSwapchain(PanelSwapchain, format, panelSize))
		{
			error = "the swapchains could not be created";
			DestroySession();
			return false;
		}

		if (Actions)
		{
			XrSessionActionSetsAttachInfo attach{ XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO };
			attach.countActionSets = 1;
			attach.actionSets = &Actions;
			Check(xrAttachSessionActionSets(Session, &attach), "xrAttachSessionActionSets");
		}

		// A pose is only ever read through a space that follows it.
		for (auto& [action, space] : PoseSpaces)
		{
			XrActionSpaceCreateInfo create{ XR_TYPE_ACTION_SPACE_CREATE_INFO };
			create.action = action;
			create.poseInActionSpace = { { 0, 0, 0, 1 }, { 0, 0, 0 } };
			if (!Check(xrCreateActionSpace(Session, &create, &space), "xrCreateActionSpace"))
				space = XR_NULL_HANDLE;
		}
		return true;
	}

	void OpenXR::DestroySession()
	{
		if (!Session)
			return;

		Open = false;
		DestroySwapchain(EyeSwapchains[0]);
		DestroySwapchain(EyeSwapchains[1]);
		DestroySwapchain(PanelSwapchain);

		for (auto& [action, space] : PoseSpaces)
		{
			if (space)
				xrDestroySpace(space);
			space = XR_NULL_HANDLE;
		}
		if (LocalSpace)
			xrDestroySpace(LocalSpace);
		if (ViewSpace)
			xrDestroySpace(ViewSpace);
		LocalSpace = ViewSpace = XR_NULL_HANDLE;

		// Destroying a running session is allowed, and what a device restart needs: waiting for the
		// runtime to walk it down to stopping would hold the restart for however long that takes.
		xrDestroySession(Session);
		Session = XR_NULL_HANDLE;
		State = XR_SESSION_STATE_UNKNOWN;
		Started = false;
		Synced = false;
		HasPose = false;
	}

	bool OpenXR::CreateSwapchain(XRSwapchain& swapchain, int64_t format, const glm::ivec2& size)
	{
		XrSwapchainCreateInfo info{ XR_TYPE_SWAPCHAIN_CREATE_INFO };
		info.usageFlags = XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT | XR_SWAPCHAIN_USAGE_SAMPLED_BIT
			| XR_SWAPCHAIN_USAGE_TRANSFER_DST_BIT;
		info.format = format;
		info.sampleCount = 1;
		info.width = size.x;
		info.height = size.y;
		info.faceCount = 1;
		info.arraySize = 1;
		info.mipCount = 1;

		if (!Check(xrCreateSwapchain(Session, &info, &swapchain.Handle), "xrCreateSwapchain"))
		{
			swapchain.Handle = XR_NULL_HANDLE;
			return false;
		}

		swapchain.Width = size.x;
		swapchain.Height = size.y;
		return Graphics->Images(swapchain.Handle, swapchain.Images);
	}

	void OpenXR::DestroySwapchain(XRSwapchain& swapchain)
	{
		if (swapchain.Handle)
			xrDestroySwapchain(swapchain.Handle);
		swapchain = {};
	}

	XrAction OpenXR::CreateAction(const char* name, const char* localized, XrActionType type)
	{
		if (!Instance || Session)
			return XR_NULL_HANDLE;

		if (!Actions)
		{
			XrActionSetCreateInfo set{ XR_TYPE_ACTION_SET_CREATE_INFO };
			strcpy_s(set.actionSetName, "gameplay");
			strcpy_s(set.localizedActionSetName, "Gameplay");
			if (!Check(xrCreateActionSet(Instance, &set, &Actions), "xrCreateActionSet"))
			{
				Actions = XR_NULL_HANDLE;
				return XR_NULL_HANDLE;
			}
		}

		XrActionCreateInfo info{ XR_TYPE_ACTION_CREATE_INFO };
		info.actionType = type;
		strcpy_s(info.actionName, name);
		strcpy_s(info.localizedActionName, localized);

		XrAction action = XR_NULL_HANDLE;
		if (!Check(xrCreateAction(Actions, &info, &action), name))
			return XR_NULL_HANDLE;
		if (type == XR_ACTION_TYPE_POSE_INPUT)
			PoseSpaces.emplace_back(action, XR_NULL_HANDLE);
		return action;
	}

	void OpenXR::SuggestBindings(const char* profile, std::initializer_list<XRBinding> bindings)
	{
		if (!Instance)
			return;

		std::vector<XrActionSuggestedBinding> suggested;
		for (const XRBinding& binding : bindings)
		{
			if (binding.Action)
				suggested.push_back({ binding.Action, Path(binding.Path) });
		}
		if (suggested.empty())
			return;

		XrInteractionProfileSuggestedBinding info{ XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING };
		info.interactionProfile = Path(profile);
		info.countSuggestedBindings = static_cast<uint32_t>(suggested.size());
		info.suggestedBindings = suggested.data();
		Check(xrSuggestInteractionProfileBindings(Instance, &info), profile);
	}

	bool OpenXR::GetBoolean(XrAction action)
	{
		XrActionStateGetInfo info{ XR_TYPE_ACTION_STATE_GET_INFO };
		info.action = action;
		XrActionStateBoolean state{ XR_TYPE_ACTION_STATE_BOOLEAN };
		if (!Synced || !action || XR_FAILED(xrGetActionStateBoolean(Session, &info, &state)) || !state.isActive)
			return false;
		return state.currentState;
	}

	float OpenXR::GetFloat(XrAction action)
	{
		XrActionStateGetInfo info{ XR_TYPE_ACTION_STATE_GET_INFO };
		info.action = action;
		XrActionStateFloat state{ XR_TYPE_ACTION_STATE_FLOAT };
		if (!Synced || !action || XR_FAILED(xrGetActionStateFloat(Session, &info, &state)) || !state.isActive)
			return 0.0f;
		return state.currentState;
	}

	vec2 OpenXR::GetVector2(XrAction action)
	{
		XrActionStateGetInfo info{ XR_TYPE_ACTION_STATE_GET_INFO };
		info.action = action;
		XrActionStateVector2f state{ XR_TYPE_ACTION_STATE_VECTOR2F };
		if (!Synced || !action || XR_FAILED(xrGetActionStateVector2f(Session, &info, &state)) || !state.isActive)
			return {};
		return { state.currentState.x, state.currentState.y };
	}

	// Where a pose action is at the time the frame is predicted to be seen, in the space the frame is
	// submitted in. False while the controller is not tracked, or off.
	bool OpenXR::GetPose(XrAction action, XrPosef& pose)
	{
		const auto entry = std::ranges::find(PoseSpaces, action, &std::pair<XrAction, XrSpace>::first);
		if (!Synced || !Open || entry == PoseSpaces.end() || !entry->second)
			return false;

		XrSpaceLocation location{ XR_TYPE_SPACE_LOCATION };
		if (XR_FAILED(xrLocateSpace(entry->second, LocalSpace, Frame.predictedDisplayTime, &location)))
			return false;

		constexpr XrSpaceLocationFlags valid = XR_SPACE_LOCATION_ORIENTATION_VALID_BIT | XR_SPACE_LOCATION_POSITION_VALID_BIT;
		if ((location.locationFlags & valid) != valid)
			return false;

		pose = location.pose;
		return true;
	}

	void OpenXR::PollEvents()
	{
		if (!Instance)
			return;

		XrEventDataBuffer event{ XR_TYPE_EVENT_DATA_BUFFER };
		while (xrPollEvent(Instance, &event) == XR_SUCCESS)
		{
			if (event.type == XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED && Session)
			{
				State = reinterpret_cast<const XrEventDataSessionStateChanged&>(event).state;
				if (OnStateChanged)
					OnStateChanged(State);

				if (State == XR_SESSION_STATE_READY)
				{
					XrSessionBeginInfo begin{ XR_TYPE_SESSION_BEGIN_INFO };
					begin.primaryViewConfigurationType = ViewConfiguration;
					Started = Check(xrBeginSession(Session, &begin), "xrBeginSession");
				}
				else if (State == XR_SESSION_STATE_STOPPING)
				{
					Open = false;
					Started = false;
					xrEndSession(Session);
				}
				else if (State == XR_SESSION_STATE_EXITING || State == XR_SESSION_STATE_LOSS_PENDING)
				{
					Open = false;
					Started = false;
					Exiting = true;
				}
			}
			else if (event.type == XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING)
			{
				Open = false;
				Started = false;
				Exiting = true;
			}
			event = { XR_TYPE_EVENT_DATA_BUFFER };
		}
	}

	// Waits for the runtime's next frame slot and opens it. Everything the frame is drawn from is
	// located here, at the time the runtime predicts it will reach the display.
	bool OpenXR::BeginFrame()
	{
		if (!Started)
			return false;

		// A frame that was never finished, from a lost device or a skipped render, still has to be
		// closed before the runtime hands out another.
		if (Open)
			EndFrame({});

		XrFrameWaitInfo wait{ XR_TYPE_FRAME_WAIT_INFO };
		Frame = { XR_TYPE_FRAME_STATE };
		if (!Check(xrWaitFrame(Session, &wait, &Frame), "xrWaitFrame"))
			return false;

		XrFrameBeginInfo begin{ XR_TYPE_FRAME_BEGIN_INFO };
		if (!Check(xrBeginFrame(Session, &begin), "xrBeginFrame"))
			return false;

		Open = true;
		Locate();
		SyncActions();
		return true;
	}

	void OpenXR::EndFrame(const XRLayers& layers)
	{
		if (!Open)
			return;
		Open = false;

		XrCompositionLayerProjectionView views[2] = {};
		XrCompositionLayerProjection projection{ XR_TYPE_COMPOSITION_LAYER_PROJECTION };
		XrCompositionLayerQuad quad{ XR_TYPE_COMPOSITION_LAYER_QUAD };
		std::vector<const XrCompositionLayerBaseHeader*> list;

		if (layers.Eyes && Frame.shouldRender)
		{
			for (int eye = 0; eye < 2; eye++)
			{
				const XRSwapchain& swapchain = EyeSwapchains[eye];

				views[eye] = { XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW };
				views[eye].pose = layers.Views[eye].Pose;
				views[eye].fov = layers.Views[eye].Fov;
				views[eye].subImage.swapchain = swapchain.Handle;
				views[eye].subImage.imageRect = { { 0, 0 }, { swapchain.Width, swapchain.Height } };
			}
			projection.space = LocalSpace;
			projection.viewCount = 2;
			projection.views = views;
			list.push_back(reinterpret_cast<const XrCompositionLayerBaseHeader*>(&projection));
		}

		if (layers.Panel && Frame.shouldRender)
		{
			quad.layerFlags = XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT;
			quad.space = ViewSpace;
			quad.eyeVisibility = XR_EYE_VISIBILITY_BOTH;
			quad.subImage.swapchain = PanelSwapchain.Handle;
			quad.subImage.imageRect = { { 0, 0 }, { PanelSwapchain.Width, PanelSwapchain.Height } };
			quad.pose = { { 0, 0, 0, 1 }, { 0, 0, -layers.PanelDistance } };
			quad.size = { layers.PanelSize.x, layers.PanelSize.y };
			list.push_back(reinterpret_cast<const XrCompositionLayerBaseHeader*>(&quad));
		}

		XrFrameEndInfo end{ XR_TYPE_FRAME_END_INFO };
		end.displayTime = Frame.predictedDisplayTime;
		end.environmentBlendMode = BlendMode;
		end.layerCount = static_cast<uint32_t>(list.size());
		end.layers = list.empty() ? nullptr : list.data();
		Check(xrEndFrame(Session, &end), "xrEndFrame");
	}

	void* OpenXR::Acquire(XRSwapchain& swapchain)
	{
		if (!swapchain.Handle)
			return nullptr;

		uint32_t index = 0;
		XrSwapchainImageAcquireInfo acquire{ XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO };
		if (!Check(xrAcquireSwapchainImage(swapchain.Handle, &acquire, &index), "xrAcquireSwapchainImage"))
			return nullptr;

		XrSwapchainImageWaitInfo wait{ XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO };
		wait.timeout = XR_INFINITE_DURATION;
		if (!Check(xrWaitSwapchainImage(swapchain.Handle, &wait), "xrWaitSwapchainImage"))
		{
			Release(swapchain);
			return nullptr;
		}
		return index < swapchain.Images.size() ? swapchain.Images[index] : nullptr;
	}

	void OpenXR::Release(XRSwapchain& swapchain)
	{
		XrSwapchainImageReleaseInfo release{ XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
		xrReleaseSwapchainImage(swapchain.Handle, &release);
	}

	void OpenXR::Locate()
	{
		XrViewLocateInfo info{ XR_TYPE_VIEW_LOCATE_INFO };
		info.viewConfigurationType = ViewConfiguration;
		info.displayTime = Frame.predictedDisplayTime;
		info.space = LocalSpace;

		XrViewState state{ XR_TYPE_VIEW_STATE };
		XrView views[2] = { { XR_TYPE_VIEW }, { XR_TYPE_VIEW } };
		uint32_t count = 0;

		HasPose = false;
		if (XR_FAILED(xrLocateViews(Session, &info, &state, 2, &count, views)) || count != 2)
			return;
		if (!(state.viewStateFlags & XR_VIEW_STATE_ORIENTATION_VALID_BIT))
			return;

		for (int eye = 0; eye < 2; eye++)
		{
			Views[eye].Pose = views[eye].pose;
			Views[eye].Fov = views[eye].fov;
		}

		XrSpaceLocation head{ XR_TYPE_SPACE_LOCATION };
		if (XR_SUCCEEDED(xrLocateSpace(ViewSpace, LocalSpace, Frame.predictedDisplayTime, &head))
			&& (head.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT))
		{
			HeadPose = head.pose;
		}
		HasPose = true;
	}

	// Only a focused session receives input; the controllers read as idle otherwise.
	void OpenXR::SyncActions()
	{
		Synced = false;
		if (!Actions || State != XR_SESSION_STATE_FOCUSED)
			return;

		XrActiveActionSet active{ Actions, XR_NULL_PATH };
		XrActionsSyncInfo sync{ XR_TYPE_ACTIONS_SYNC_INFO };
		sync.countActiveActionSets = 1;
		sync.activeActionSets = &active;
		Synced = XR_SUCCEEDED(xrSyncActions(Session, &sync));
	}

	const char* OpenXR::StateName(XrSessionState state)
	{
		switch (state)
		{
		case XR_SESSION_STATE_IDLE:
			return "idle";
		case XR_SESSION_STATE_READY:
			return "ready";
		case XR_SESSION_STATE_SYNCHRONIZED:
			return "synchronized";
		case XR_SESSION_STATE_VISIBLE:
			return "visible";
		case XR_SESSION_STATE_FOCUSED:
			return "focused";
		case XR_SESSION_STATE_STOPPING:
			return "stopping";
		case XR_SESSION_STATE_LOSS_PENDING:
			return "lost";
		case XR_SESSION_STATE_EXITING:
			return "exiting";
		default:
			return "unknown";
		}
	}

	// For a graphics binding to reach its own API's functions.
	PFN_xrVoidFunction OpenXR::Function(const char* name)
	{
		PFN_xrVoidFunction function = nullptr;
		if (!Instance || XR_FAILED(xrGetInstanceProcAddr(Instance, name, &function)))
			return nullptr;
		return function;
	}

	XrPath OpenXR::Path(const char* path)
	{
		XrPath result = XR_NULL_PATH;
		xrStringToPath(Instance, path, &result);
		return result;
	}

	bool OpenXR::Check(XrResult result, const char* what)
	{
		if (XR_SUCCEEDED(result))
			return true;

		char name[XR_MAX_RESULT_STRING_SIZE] = {};
		if (!Instance || XR_FAILED(xrResultToString(Instance, result, name)))
			snprintf(name, sizeof(name), "%d", static_cast<int>(result));

		Log::WriteLine(Channel::Error, "OpenXR: {} failed: {}", what, name);
		return false;
	}

	// The runtime is done with the session: another application took the headset, or the player quit
	// from its menu. SteamVR only lets go of a process once it drops its instance, and kills it otherwise.
	bool OpenXR::Ended()
	{
		return Exiting;
	}

	bool OpenXR::Focused()
	{
		return State == XR_SESSION_STATE_FOCUSED;
	}

	bool OpenXR::FrameOpen()
	{
		return Open;
	}

	bool OpenXR::Located()
	{
		return HasPose;
	}

	const XRView& OpenXR::View(int eye)
	{
		return Views[eye];
	}

	const XrPosef& OpenXR::Head()
	{
		return HeadPose;
	}

	glm::ivec2 OpenXR::RecommendedSize()
	{
		return { static_cast<int>(Configurations[0].recommendedImageRectWidth),
			static_cast<int>(Configurations[0].recommendedImageRectHeight) };
	}

	glm::ivec2 OpenXR::MaximumSize()
	{
		return { static_cast<int>(Configurations[0].maxImageRectWidth),
			static_cast<int>(Configurations[0].maxImageRectHeight) };
	}

	const std::string& OpenXR::RuntimeName()
	{
		return Runtime;
	}
}
