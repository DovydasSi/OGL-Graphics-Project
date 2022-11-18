#pragma once

#include "../nclgl/SceneNode.h"
#include "../nclgl/Camera.h"

class WaterNode : public SceneNode
{
public:
	WaterNode(Shader* s, Mesh* m, Camera* cam, GLuint cube, DirLight* l);
	~WaterNode();

	void Update(float dt) override;
	void Draw(const OGLRenderer& r) override;
protected:
	GLuint waterTexture;
	float waterRotate;
	float waterCycle;

	DirLight* light;
	Camera* camera;
	GLuint cubeMap;
};