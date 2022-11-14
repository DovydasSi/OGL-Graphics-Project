#include "AnimSceneNode.h"

AnimSceneNode::AnimSceneNode(Shader* s, Mesh* m, MeshAnimation* a, MeshMaterial* mm) :
	MatSceneNode(s, m, mm)
{
	curr_anim = a;

	currentFrame = 0;
	frameTime = 0.0f;
}

AnimSceneNode::~AnimSceneNode()
{
	delete curr_anim;
}

void AnimSceneNode::Update(float dt)
{
	frameTime -= dt;
	while (frameTime < 0.0f) {
		currentFrame = (currentFrame + 1) % curr_anim->GetFrameCount();
		frameTime += 1.0f / curr_anim->GetFrameRate();
	}

	MatSceneNode::Update(dt);
}

void AnimSceneNode::Draw(const OGLRenderer& r)
{
	vector <Matrix4 > frameMatrices;
	const Matrix4* invBindPose = mesh->GetInverseBindPose();
	const Matrix4* frameData = curr_anim->GetJointData(currentFrame);
	for (unsigned int i = 0; i < mesh->GetJointCount(); ++i) {
		frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
	}
	int j = glGetUniformLocation(shader->GetProgram(), "joints");
	glUniformMatrix4fv(j, frameMatrices.size(), false, (float*)frameMatrices.data());


	MatSceneNode::Draw(r);
}

void AnimSceneNode::ChangeAnimation(const std::string anim_file)
{}