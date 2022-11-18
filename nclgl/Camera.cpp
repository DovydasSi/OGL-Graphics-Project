#include "Camera.h"
#include "Window.h"
#include <algorithm>

Camera::Camera() : yaw(0.0f), pitch(0.0f), mode(CAMERA_MODE::CM_AUTOMATIC)
{
	is_travelling = false;
	paths = vector<CameraPath>();
	LoadPaths();
	nextPath = *paths.begin();
	revealed_index = 0;
}

Camera::Camera(float pit, float yw, Vector3 pos) : pitch(pit), yaw(yw), position(pos), mode(CAMERA_MODE::CM_AUTOMATIC)
{
	is_travelling = false;
	paths = vector<CameraPath>();
	LoadPaths();
	nextPath = *paths.begin();
	revealed_index = 0;
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
		if (!is_travelling)
		{
			Vector3 pos = nextPath.position - position;
			float pit = nextPath.pitch - pitch;
			float yw= nextPath.yaw - yaw;

			timeTravelled = 0.0f;
			path_to_next = pos;
			pitch_to_next = pit;
			yaw_to_next = yw;
			is_travelling = true;
		}
		
		position += path_to_next/nextPath.speed;
		pitch += pitch_to_next / nextPath.speed;
		yaw += yaw_to_next / nextPath.speed;

		timeTravelled++;

		if (timeTravelled == nextPath.speed)
		{
			is_travelling = false;
			nextPath = TravelToNext();
		}
	}

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_C))
	{
		timeTravelled = 0.0f;
		is_travelling = false;
		mode = mode == CAMERA_MODE::CM_AUTOMATIC ? CAMERA_MODE::CM_MANUAL : CAMERA_MODE::CM_AUTOMATIC;
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

void Camera::LoadPaths()
{
	paths.emplace_back(Vector3(-30.1676f, 319.969f, 832.368f), -26.28f, 3.44f, 120.f);
	paths.emplace_back(Vector3(317.37f, 319.969f, 896.799f), -25.23f, 3.51001f, 180.f);
	paths.emplace_back(Vector3(574.047f, 319.969f, 842.39f), -23.13f, 26.33f, 120.f);
	paths.emplace_back(Vector3(435.423f, 229.887f, 519.582f), -18.02f, 29.48f, 120.f);
	paths.emplace_back(Vector3(536.449f, 229.887f, 383.299f), -13.61f, 94.65f, 120.f);
	paths.emplace_back(Vector3(536.449f, 229.887f, 383.299f), -0.450008f, 272.65f, 120.f);
	paths.emplace_back(Vector3(546.295f, 69.7495f, 381.168f), -15.43f, 274.33f, 120.f);
	paths.emplace_back(Vector3(848.283f, 54.7635f, 558.978f), -7.87001f, 357.7f, 120.f);
	paths.emplace_back(Vector3(1060.26f, 305.001f, 73.6251f), -19.28f, 117.19f, 120.f);
	paths.emplace_back(Vector3(605.275f, 305.001f, -13.2966f), -20.19f, 151.63f, 120.f);
	paths.emplace_back(Vector3(-113.937f, 405.086f, 241.766f), -30.97f, 261.461f, 120.f);
	paths.emplace_back(Vector3(-111.319f, 329.99f, 536.434f), -23.27f, 293.031f, 120.f);
	paths.emplace_back(Vector3(199.384f, 199.88f, 548.548f), -8.57f, 322.781f, 120.f);
	paths.emplace_back(Vector3(61.9532f, 310.002f, 875.863f), -15.22f, 329.291f, 120.f);
}

CameraPath Camera::TravelToNext()
{
	if (revealed_index >= 0 && revealed_index < paths.size() - 1)
	{
		return paths[++revealed_index];
	}
	else if (revealed_index < paths.size())
	{
		revealed_index = 0;
		return paths[0];
	}

	return CameraPath();
}