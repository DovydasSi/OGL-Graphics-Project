#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include <vector>

struct CameraPath
{
	Vector3 position;
	float pitch;
	float yaw;
	float speed;

	CameraPath() : position(Vector3(0, 0, 0)), pitch(0), yaw(0), speed(1) {}

	CameraPath(Vector3 pos, float pit, float yw, float sp) :
		position(pos), pitch(pit), yaw(yw), speed(sp) {}
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
	void		SetPitch(float val)			{ pitch = val; }

	void		SetMode(CAMERA_MODE m)		{ mode = m; }

protected:
	float yaw;
	float pitch;
	Vector3 position;
	CAMERA_MODE mode;

	void		HandleManualControls(float speed);
	void		LoadPaths();
	CameraPath	TravelToNext();

	CameraPath nextPath;
	std::vector<CameraPath> paths;
	bool is_travelling;

	Vector3 path_to_next;
	float yaw_to_next;
	int yaw_multiplier;
	float pitch_to_next;
	float timeTravelled;
	int revealed_index;
};