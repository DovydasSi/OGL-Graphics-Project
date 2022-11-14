#include "CubeRobot.h"

CubeRobot::CubeRobot(Mesh* cube)
{
	//SetMesh(cube);

	SceneNode* body = new SceneNode(nullptr, cube, Vector4(1, 0, 0, 1));
	body->SetModelScale(Vector3(10, 15, 5));
	body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
	AddChild(body);

	head = new SceneNode(nullptr, cube, Vector4(0, 1, 0, 1));
	head->SetModelScale(Vector3(5, 5, 5));
	head->SetTransform(Matrix4::Translation(Vector3(0, 30, 0)));
	body->AddChild(head);

	leftArm = new SceneNode(nullptr, cube, Vector4(0, 0, 1, 1));
	leftArm->SetModelScale(Vector3(3, -18, 3));
	leftArm->SetTransform(Matrix4::Translation(Vector3(-12, 30, -1)));
	body->AddChild(leftArm);

	rightArm = new SceneNode(nullptr, cube, Vector4(0, 0, 1, 1));
	rightArm->SetModelScale(Vector3(3, -18, 3));
	rightArm->SetTransform(Matrix4::Translation(Vector3(12, 30, -1)));
	body->AddChild(rightArm);

	hips = new SceneNode(nullptr, cube, Vector4(0, 1, 0.5f, 1));
	hips->SetModelScale(Vector3(10, 2.5f, 5));
	hips->SetTransform(Matrix4::Translation(Vector3(0, -5, 0)));
	body->AddChild(hips);

	SceneNode* leftLeg = new SceneNode(nullptr, cube, Vector4(0, 0, 1, 1));
	leftLeg->SetModelScale(Vector3(3, -17.5, 3));
	leftLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
	hips->AddChild(leftLeg);

	SceneNode* rightLeg = new SceneNode(nullptr, cube, Vector4(0, 0, 1, 1));
	rightLeg->SetModelScale(Vector3(3, -17.5, 3));
	rightLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
	hips->AddChild(rightLeg);


	body->SetBoundingRadius(15.0f);
	head->SetBoundingRadius(5.0f);
	
	leftArm->SetBoundingRadius(18.0f);
	rightArm->SetBoundingRadius(18.0f);
	
	leftLeg->SetBoundingRadius(18.0f);
	rightLeg->SetBoundingRadius(18.0f);

	hips->SetBoundingRadius(10);
}

void CubeRobot::Update(float dt)
{
	float speed = dt * 30.0f;

	transform = transform * Matrix4::Rotation(speed, Vector3(0, 1, 0));

	head->SetTransform(head->GetTransform() * Matrix4::Rotation(-speed, Vector3(0, 1, 0)));
	leftArm->SetTransform(leftArm->GetTransform() * Matrix4::Rotation(-speed, Vector3(1, 0, 0)));
	rightArm->SetTransform(rightArm->GetTransform() * Matrix4::Rotation(speed, Vector3(1, 0, 0)));
	hips->SetTransform(hips->GetTransform() * Matrix4::Rotation(speed, Vector3(1, 1, 1)));

	SceneNode::Update(dt);
}

