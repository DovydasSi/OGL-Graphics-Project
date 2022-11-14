#pragma once
#include "Light.h"

class DirLight : public Light
{
public:
	DirLight(Vector3 dir, Vector4 colour, Vector4 speccolour) : Light(Vector3(), colour, speccolour, 0.0f),
		direction(dir) 
	{
		type = Directional; 
	}

	Vector3 GetDirection() const { return direction; }
	void SetDirection(const Vector3& val) { direction = val; }
protected:
	Vector3 direction;
};