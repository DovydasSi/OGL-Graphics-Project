#pragma once
#include "Vector4.h"
#include "Vector3.h"

class Light {
public:
	Light() {} // Default constructor , we’ll be needing this later!
	Light(const Vector3 & position, const Vector4 & colour, const Vector4 & speccolour, float radius) {
		this->position = position;
		this->colour = colour;
		this->specColour = speccolour;
		this->radius = radius;

		type = Point;
	}
	
	~Light(void) {};
	
	Vector3 GetPosition() const { return position; }
	void SetPosition(const Vector3 & val) { position = val; }
	
	float GetRadius() const { return radius; }
	void SetRadius(float val) { radius = val; }
	Vector4 GetColour() const { return colour; }
	void SetColour(const Vector4 & val) { colour = val; }
	Vector4 GetSpecColour() const { return specColour; }
	void SetSpecColour(const Vector4 & val) { specColour = val; }

	enum LIGHT_TYPE
	{
		Point = 0,
		Directional = 1,
		Spot = 2
	};

	LIGHT_TYPE Get_Type() const { return type; }
	
protected:
	Vector3 position;
	float radius;
	Vector4 colour;
	Vector4 specColour;
	LIGHT_TYPE type;
};