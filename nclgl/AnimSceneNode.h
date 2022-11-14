#pragma once
#include "MatSceneNode.h"
#include "Shader.h"
#include "MeshAnimation.h"
#include <string>

class AnimSceneNode : public MatSceneNode
{
public:
	AnimSceneNode(Shader * s, Mesh* m, MeshAnimation* a, MeshMaterial* mm);
	~AnimSceneNode();

	void Update(float dt) override;
	void Draw(const OGLRenderer& r) override;
protected:
	MeshAnimation* curr_anim;
	vector<MeshAnimation* > animations;

	int currentFrame;
	float frameTime;

	void ChangeAnimation(const std::string animfile);
};