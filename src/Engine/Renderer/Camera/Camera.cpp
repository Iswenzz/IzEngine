#include "Camera.hpp"

namespace IzEngine
{
	Camera Camera::CreateOrthographic(float width, float height, float nearClip, float farClip)
	{
		Camera camera;
		camera.SetOrthographic(width, height, nearClip, farClip);
		return camera;
	}

	Camera Camera::CreatePerspective(float fov, float aspectRatio, float nearClip, float farClip)
	{
		Camera camera;
		camera.SetPerspective(fov, aspectRatio, nearClip, farClip);
		return camera;
	}

	void Camera::SetOrthographic(float width, float height, float nearClip, float farClip)
	{
		Mode = CameraMode::Orthographic;
		Width = width;
		Height = height;
		NearClip = nearClip;
		FarClip = farClip;

		RecalculateProjection();
		RecalculateViewProjection();
	}

	void Camera::SetPerspective(float fov, float aspectRatio, float nearClip, float farClip)
	{
		Mode = CameraMode::Perspective;
		FOV = fov;
		AspectRatio = aspectRatio;
		NearClip = nearClip;
		FarClip = farClip;

		RecalculateProjection();
		RecalculateViewProjection();
	}

	void Camera::SetPosition(const vec3& position)
	{
		Position = position;
		RecalculateView();
		RecalculateViewProjection();
	}

	void Camera::SetRotation(const vec3& rotation)
	{
		Rotation = rotation;
		RecalculateView();
		RecalculateViewProjection();
	}

	void Camera::Resize(float width, float height)
	{
		Width = width;
		Height = height;
		AspectRatio = width / height;

		RecalculateProjection();
		RecalculateViewProjection();
	}

	void Camera::RecalculateProjection()
	{
		if (Mode == CameraMode::Orthographic)
		{
			Projection = glm::ortho(0.0f, Width, Height, 0.0f, NearClip, FarClip);
		}
		else
		{
			Projection = glm::perspective(glm::radians(FOV), AspectRatio, NearClip, FarClip);
		}
	}

	void Camera::RecalculateView()
	{
		mat4 transform = glm::translate(mat4(1.0f), Position)
			* glm::rotate(mat4(1.0f), glm::radians(Rotation.x), vec3(1.0f, 0.0f, 0.0f))
			* glm::rotate(mat4(1.0f), glm::radians(Rotation.y), vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(mat4(1.0f), glm::radians(Rotation.z), vec3(0.0f, 0.0f, 1.0f));

		View = glm::inverse(transform);
	}

	void Camera::RecalculateViewProjection()
	{
		ViewProjection = Projection * View;
	}

	const mat4& Camera::GetProjection() const
	{
		return Projection;
	}

	const mat4& Camera::GetView() const
	{
		return View;
	}

	const mat4& Camera::GetViewProjection() const
	{
		return ViewProjection;
	}

	const vec3& Camera::GetPosition() const
	{
		return Position;
	}

	const vec3& Camera::GetRotation() const
	{
		return Rotation;
	}

	CameraMode Camera::GetMode() const
	{
		return Mode;
	}

	float Camera::GetFOV() const
	{
		return FOV;
	}

	float Camera::GetAspectRatio() const
	{
		return AspectRatio;
	}

	float Camera::GetNearClip() const
	{
		return NearClip;
	}

	float Camera::GetFarClip() const
	{
		return FarClip;
	}

	vec3 Camera::GetForward() const
	{
		return glm::normalize(vec3(glm::cos(glm::radians(Rotation.y)) * glm::cos(glm::radians(Rotation.x)),
			glm::sin(glm::radians(Rotation.x)),
			glm::sin(glm::radians(Rotation.y)) * glm::cos(glm::radians(Rotation.x))));
	}

	vec3 Camera::GetRight() const
	{
		return glm::normalize(glm::cross(GetForward(), vec3(0.0f, 1.0f, 0.0f)));
	}

	vec3 Camera::GetUp() const
	{
		return glm::normalize(glm::cross(GetRight(), GetForward()));
	}
}
