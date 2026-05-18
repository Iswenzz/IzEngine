#pragma once
#include "Engine/Core/Common.hpp"

namespace IzEngine
{
	enum class CameraMode
	{
		Orthographic,
		Perspective
	};

	class Camera
	{
	public:
		Camera() = default;

		static Camera CreateOrthographic(float width, float height, float nearClip = -1.0f, float farClip = 1.0f);
		static Camera CreatePerspective(float fov, float aspectRatio, float nearClip = 0.1f, float farClip = 1000.0f);

		void SetOrthographic(float width, float height, float nearClip = -1.0f, float farClip = 1.0f);
		void SetPerspective(float fov, float aspectRatio, float nearClip = 0.1f, float farClip = 1000.0f);

		void SetPosition(const vec3& position);
		void SetRotation(const vec3& rotation);

		void Resize(float width, float height);

		const mat4& GetProjection() const;
		const mat4& GetView() const;
		const mat4& GetViewProjection() const;

		const vec3& GetPosition() const;
		const vec3& GetRotation() const;

		vec3 GetForward() const;
		vec3 GetRight() const;
		vec3 GetUp() const;

		CameraMode GetMode() const;
		float GetFOV() const;
		float GetAspectRatio() const;
		float GetNearClip() const;
		float GetFarClip() const;

	private:
		CameraMode Mode = CameraMode::Perspective;
		mat4 Projection = mat4(1.0f);
		mat4 View = mat4(1.0f);
		mat4 ViewProjection = mat4(1.0f);
		vec3 Position;
		vec3 Rotation;
		float FOV = 60.0f;
		float AspectRatio = 16.0f / 9.0f;
		float NearClip = 0.1f;
		float FarClip = 1000.0f;
		float Width = 1920.0f;
		float Height = 1080.0f;

		void RecalculateView();
		void RecalculateProjection();
		void RecalculateViewProjection();
	};
}
