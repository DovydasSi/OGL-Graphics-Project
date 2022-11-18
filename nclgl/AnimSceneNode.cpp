#include "AnimSceneNode.h"

AnimSceneNode::AnimSceneNode(Shader* s, Mesh* m, MeshAnimation* a, MeshMaterial* mm) :
	MatSceneNode(s, m, mm)
{
	anim = a;

	currentFrame = 0;
	frameTime = 0.0f;
}

AnimSceneNode::~AnimSceneNode()
{
	delete anim;
}

void AnimSceneNode::Update(float dt)
{
	frameTime -= dt;
	while (frameTime < 0.0f) {
		currentFrame = (currentFrame + 1) % anim->GetFrameCount();
		frameTime += 1.0f / anim->GetFrameRate();
	}

	MatSceneNode::Update(dt);
}

void AnimSceneNode::Draw(const OGLRenderer& r)
{
	vector <Matrix4 > frameMatrices;
	const Matrix4* invBindPose = mesh->GetInverseBindPose();
	const Matrix4* frameData = anim->GetJointData(currentFrame);
	for (unsigned int i = 0; i < mesh->GetJointCount(); ++i) {
		frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
	}
	int j = glGetUniformLocation(shader->GetProgram(), "joints");
	glUniformMatrix4fv(j, frameMatrices.size(), false, (float*)frameMatrices.data());


	MatSceneNode::Draw(r);
}