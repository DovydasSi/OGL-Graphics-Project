#pragma once
#include "Matrix4.h"
#include "Vector3.h"

struct CameraPath
{
	Vector3 position;
	Vector3 rotation;
	float speed;
};

class Camera
{
public:
	enum class CAMERA_MODE
	{
		CM_MANUAL = 0,
		CM_AUTOMATIC = 1
	};

	Camera();

	Camera(float pitch, float yaw, Vector3 position);

	~Camera();

	void UpdateCamera(float dt = 1.0f);

	Matrix4 BuildViewMatrix();

	Vector3		GetPosition() const			{ return position; }
	void		SetPosition(Vector3 val)	{ position = val; }

	float		GetYaw() const				{ return yaw; }
	void		SetYaw(float val)			{ yaw = val; }

	float		GetPitch()					{ return pitch; }
	void		GetPitch(float val)			{ pitch = val; }

	void		SetMode(CAMERA_MODE m)		{ mode = m; }

protected:
	float yaw;
	float pitch;
	Vector3 position;
	CAMERA_MODE mode;

	void		HandleManualControls(float speed);
	Vector3		HandleAutomaticControls(CameraPath& next);
	CameraPath cp;
};