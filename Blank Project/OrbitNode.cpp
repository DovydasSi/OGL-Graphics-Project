#include "OrbitNode.h"

OrbitNode::OrbitNode(Vector3 offset, Vector3 rotAxes) : SceneNode()
{
	radius = offset;
	rotationAxes = rotAxes;
	totalTime = 0.f;
}

void OrbitNode::Update(float dt)
{
	totalTime += dt * 30;
	transform = Matrix4::Rotation(totalTime, rotationAxes) * Matrix4::Translation(radius);

	SceneNode::Update(dt);
}