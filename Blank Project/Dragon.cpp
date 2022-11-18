#include "Dragon.h"


Dragon::Dragon(Shader* s, Mesh* m, MeshAnimation* a, MeshMaterial* mm) : AnimSceneNode(s, m, a, mm), totalTime(0.0f)
{ }


Dragon::~Dragon()
{
}

void Dragon::Update(float dt)
{
	AnimSceneNode::Update(dt);
}

void Dragon::Draw(const OGLRenderer& r)
{
	AnimSceneNode::Draw(r);
}