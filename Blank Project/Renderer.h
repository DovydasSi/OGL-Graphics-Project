#pragma once
#include "../NCLGL/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/AnimSceneNode.h"
#include "WorldHeightMap.h"

//Coursework Renderer

class Renderer : public OGLRenderer	{
public:
	Renderer(Window &parent);
	 ~Renderer(void);
	 void RenderScene()				override;
	 void UpdateScene(float msec)	override;

	 void DrawNode(SceneNode* n);

protected:
	WorldHeightMap* heightMap;
	Camera* camera;
	Shader* world_shader;
	Shader* skinning_shader;
	Shader* object_shader;
	DirLight* light;

	GLuint ubo;

	SceneNode* root;

	float currentTime;
};
