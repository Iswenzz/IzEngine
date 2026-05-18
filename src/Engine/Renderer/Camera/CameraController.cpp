#include "CameraController.hpp"

#include "Engine/Core/Input/Input.hpp"
#include "Engine/Core/Input/Keyboard.hpp"
#include "Engine/Core/Input/Mouse.hpp"

namespace IzEngine
{
	CameraController::CameraController(Camera& camera, const CameraControllerSettings& settings)
		: Cam(camera), Settings(settings)
	{
	}

	void CameraController::OnUpdate(float deltaTime)
	{
		if (Cam.GetMode() == CameraMode::Orthographic)
			UpdateOrthographic(deltaTime);
		else
			UpdatePerspective(deltaTime);
	}

	void CameraController::OnResize(float width, float height)
	{
		Cam.Resize(width, height);
	}

	void CameraController::UpdateOrthographic(float deltaTime)
	{
		vec3 position = Cam.GetPosition();

		if (Input::IsDown(Key_W))
			position.y -= Settings.TranslationSpeed * Zoom * deltaTime;
		if (Input::IsDown(Key_S))
			position.y += Settings.TranslationSpeed * Zoom * deltaTime;
		if (Input::IsDown(Key_A))
			position.x -= Settings.TranslationSpeed * Zoom * deltaTime;
		if (Input::IsDown(Key_D))
			position.x += Settings.TranslationSpeed * Zoom * deltaTime;

		float scroll = Mouse::ScrollDelta;
		if (scroll != 0.0f)
		{
			Zoom -= scroll * Settings.ZoomSpeed * 0.1f;
			Zoom = glm::clamp(Zoom, Settings.MinZoom, Settings.MaxZoom);
			Cam.SetOrthographic(Cam.GetAspectRatio() * 2.0f * Zoom, 2.0f * Zoom, Cam.GetNearClip(), Cam.GetFarClip());
		}
		Cam.SetPosition(position);
	}

	void CameraController::UpdatePerspective(float deltaTime)
	{
		vec3 position = Cam.GetPosition();
		vec3 rotation = Cam.GetRotation();

		if (Input::IsDown(Button_Right))
		{
			vec2 mousePos = Mouse::Position;
			vec2 delta = (mousePos - LastMousePosition) * Settings.RotationSpeed;
			LastMousePosition = mousePos;

			rotation.y += delta.x;
			rotation.x = glm::clamp(rotation.x - delta.y, -89.0f, 89.0f);

			Cam.SetRotation(rotation);
		}
		else
		{
			LastMousePosition = Mouse::Position;
		}
		vec3 forward = Cam.GetForward();
		vec3 right = Cam.GetRight();
		vec3 up = Cam.GetUp();
		float speed = Settings.TranslationSpeed * deltaTime;

		if (Input::IsDown(Key_W))
			position += forward * speed;
		if (Input::IsDown(Key_S))
			position -= forward * speed;
		if (Input::IsDown(Key_A))
			position -= right * speed;
		if (Input::IsDown(Key_D))
			position += right * speed;
		if (Input::IsDown(Key_Q))
			position -= up * speed;
		if (Input::IsDown(Key_E))
			position += up * speed;

		Cam.SetPosition(position);
	}

	Camera& CameraController::GetCamera()
	{
		return Cam;
	}

	const CameraControllerSettings& CameraController::GetSettings() const
	{
		return Settings;
	}

	void CameraController::SetSettings(const CameraControllerSettings& settings)
	{
		Settings = settings;
	}
}
