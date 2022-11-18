#pragma once

#include "..//nclgl/AnimSceneNode.h"

class Dragon : public AnimSceneNode
{
public:
	//Dragon(Shader* s);
	Dragon(Shader* s, Mesh* m, MeshAnimation * a, MeshMaterial* mm);
	~Dragon();

	void Update(float dt) override;
	void Draw(const OGLRenderer& r) override;

protected:
	float totalTime;
};