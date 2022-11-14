#include "Camera.h"
#include "Window.h"
#include <algorithm>

Camera::Camera() : yaw(0.0f), pitch(0.0f), mode(CAMERA_MODE::CM_MANUAL)
{
	cp.position = Vector3(0, 50, 0);
	cp.speed = 5;
}

Camera::Camera(float pit, float yw, Vector3 pos) : pitch(pit), yaw(yw), position(pos), mode(CAMERA_MODE::CM_MANUAL)
{
	cp.position = Vector3(500, 30, 0);
	cp.speed = 600;
}

Camera::~Camera() {}

void Camera::UpdateCamera(float dt)
{
	float speed = dt * 300.0f;

	if (mode == CAMERA_MODE::CM_MANUAL)
	{
		pitch -= (Window::GetMouse()->GetRelativePosition().y);
		yaw -= (Window::GetMouse()->GetRelativePosition().x);

		pitch = std::min(pitch, 90.0f);
		pitch = std::max(pitch, -90.0f);

		if (yaw < 0)
		{
			yaw += 360.0f;
		}
		if (yaw > 360.0f)
		{
			yaw -= 360.0f;
		}

		HandleManualControls(speed);
	}
	else
	{
		Vector3 p = HandleAutomaticControls( cp );
		if (p == Vector3(0, 0, 0))
		{
			mode = CAMERA_MODE::CM_MANUAL;
		}
	}
}

Matrix4 Camera::BuildViewMatrix()
{
	return	Matrix4::Rotation( -pitch,	Vector3(1, 0, 0)) *
			Matrix4::Rotation( -yaw,	Vector3(0, 1, 0)) *
			Matrix4::Translation(-position);
}

void Camera::HandleManualControls(float speed)
{
	Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0));
	Vector3 forward = rotation * Vector3(0, 0, -1);
	Vector3 right = rotation * Vector3(1, 0, 0);

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_W))
	{
		position += forward * speed;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S))
	{
		position -= forward * speed;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A))
	{
		position -= right * speed;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D))
	{
		position += right * speed;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT))
	{
		position.y += speed;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE))
	{
		position.y -= speed;
	}
}

Vector3 Camera::HandleAutomaticControls(CameraPath& next)
{
	return Vector3();
}