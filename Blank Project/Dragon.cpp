#include "Dragon.h"


Dragon::Dragon(Shader * s, Mesh* m, MeshAnimation* a, MeshMaterial* mm) : AnimSceneNode(s, m, a, mm)
{ }

Dragon::~Dragon()
{
}

void Dragon::Update(float dt)
{
	// Move the dragon
	

	AnimSceneNode::Update(dt);
}

void Dragon::Draw(const OGLRenderer& r)
{
	AnimSceneNode::Draw(r);
}