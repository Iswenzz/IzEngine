#pragma once
#include "Camera.hpp"

namespace IzEngine
{
	struct CameraControllerSettings
	{
		float TranslationSpeed = 5.0f;
		float RotationSpeed = 0.3f;
		float ZoomSpeed = 1.0f;
		float MinZoom = 0.1f;
		float MaxZoom = 100.0f;
	};

	class CameraController
	{
	public:
		CameraController(Camera& camera, const CameraControllerSettings& settings = {});

		void OnUpdate(float deltaTime);
		void OnResize(float width, float height);

		Camera& GetCamera();
		const CameraControllerSettings& GetSettings() const;
		void SetSettings(const CameraControllerSettings& settings);

	private:
		Camera& Cam;
		CameraControllerSettings Settings;
		vec2 LastMousePosition;
		float Zoom = 1.0f;

		void UpdateOrthographic(float deltaTime);
		void UpdatePerspective(float deltaTime);
	};
}
