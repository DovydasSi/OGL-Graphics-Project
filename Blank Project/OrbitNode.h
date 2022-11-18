#pragma once

#include "../nclgl/SceneNode.h"

//Orbits around parent node

class OrbitNode : public SceneNode
{
public:
	OrbitNode(Vector3 offset, Vector3 rotAxes);
	~OrbitNode();

	void Update(float dt) override;

protected:
	float totalTime;
	Vector3 radius;
	Vector3 rotationAxes;
};