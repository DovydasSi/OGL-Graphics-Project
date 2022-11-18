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

protected:
	void LoadShaders();

	void DrawNode(SceneNode* n);
	void DrawSkybox();
	void DrawShadowScene();
	void DrawNodeMesh(SceneNode* n, Shader* s);
	void ManageInputs();

protected:
	Shader* world_shader;
	Shader* skinning_shader;
	Shader* object_shader;
	Shader* cubemap_shader;
	Shader* water_shader;
	Shader* shadow_shader;

	WorldHeightMap* heightMap;
	Mesh* basicQuad;
	Camera* camera;
	
	Mesh* dragon_mesh; 
	MeshMaterial* dragon_mat; 
	MeshAnimation* dragon_anim;

	Mesh* tower_mesh; 
	MeshMaterial* tower_mat;

	DirLight* light;

	GLuint ubo;
	GLuint cubeMap;
	GLuint shadowTex;
	GLuint shadowFBO;

	SceneNode* root;

	float currentTime;

	bool spin_camera;
};
