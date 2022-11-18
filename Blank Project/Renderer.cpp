#include "Renderer.h"
#include "Dragon.h"
#include "ObjectLookup.h"
#include "OrbitNode.h"
#include "WaterNode.h"

//Coursework Renderer

#define SHADOWSIZE 2048

Renderer::Renderer(Window& parent) : OGLRenderer(parent), currentTime(0), spin_camera(false)
{
	LoadShaders();

	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"skybox_left.psd", TEXTUREDIR"skybox_right.psd",
		TEXTUREDIR"skybox_up.psd", TEXTUREDIR"rusted_down.jpg", // (different on purpose)
		TEXTUREDIR"skybox_front.psd", TEXTUREDIR"skybox_back.psd",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	basicQuad = Mesh::GenerateQuad();

	light = new DirLight(Vector3(0.5f, 0.5f, 0), Vector4(1, 1, 1, 1), Vector4(0, 1, 0, 1));
	
	heightMap = new WorldHeightMap(TEXTUREDIR"HeightMap.png", world_shader, light);

	camera = new Camera(-40, 70, Vector3());

	Vector3 dimensions = heightMap->GetHeightmapSize();
	camera->SetPosition(dimensions * Vector3(0.5f, 2, 0.5f));

	root = new SceneNode();
	root->SetModelScale(Vector3(2, 2, 2));
	root->SetTransform(Matrix4::Translation(dimensions * Vector3(0.5f, 0.5f, 0.5f)));

	SceneNode* water = new WaterNode(water_shader, basicQuad, camera, cubeMap, light);
	water->SetTransform(Matrix4::Translation(Vector3(500.0f, -110.0f, 0.0f)) * Matrix4::Rotation(-90, Vector3(1, 0, 0)));
	water->SetModelScale(Vector3(0.5f * dimensions.x, 350.f, 0.5f*dimensions.z));
	root->AddChild(water);

	getTowerInfo(&tower_mesh, &tower_mat);
	SceneNode* tower = new MatSceneNode(object_shader, tower_mesh, tower_mat);
	tower->SetTransform(Matrix4::Translation(Vector3(30, 45, -45)));
	tower->SetModelScale(root->GetModelScale() * Vector3(1.2f, 1.2f, 1.2f));
	root->AddChild(tower);

	getRedDragonInfo(&dragon_mesh, &dragon_anim, &dragon_mat);
	SceneNode* orbit = new OrbitNode(Vector3(-150, 0, 0), Vector3(0, 1, 0));
	Dragon* din_tei = new Dragon(skinning_shader, dragon_mesh, dragon_anim, dragon_mat);
	din_tei->SetModelScale(Vector3(2.f, 2.f, 2.f));
	orbit->AddChild(din_tei);
	tower->AddChild(orbit);

	for (int i = 0; i < 10; i++)
	{
		SceneNode* orbit = new OrbitNode(Vector3(-750 - i * 50, i* 100, i * 30), Vector3(0, 1, 1));

		din_tei = new Dragon(skinning_shader, dragon_mesh, dragon_anim, dragon_mat);
		din_tei->SetModelScale(Vector3(2.f, 2.f, 2.f));
		orbit->AddChild(din_tei);

		root->AddChild(orbit);
	}

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	//Unused Shadow code
	{
		glGenTextures(1, &shadowTex);
		glBindTexture(GL_TEXTURE_2D, shadowTex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glBindTexture(GL_TEXTURE_2D, 0);

		glGenFramebuffers(1, &shadowFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
		glDrawBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	init = true;
}

Renderer::~Renderer(void)	{
	delete light;
	delete camera;
	delete heightMap;

	delete world_shader;
	delete skinning_shader;
	delete object_shader;
	delete cubemap_shader;
	delete water_shader;
	delete shadow_shader;
	delete basicQuad;

	delete dragon_mesh;
	delete dragon_mat;
	delete dragon_anim;
	delete tower_mesh;
	delete tower_mat;

	delete root;

	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
}

void Renderer::UpdateScene(float dt) 
{
	ManageInputs();
	camera->UpdateCamera(dt);
	root->Update(dt);
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	if (spin_camera)
	{
		currentTime += dt;
		light->SetDirection( Vector3(cos(currentTime), sin(currentTime), 0 ));
		if (currentTime > 2 * PI)
		{
			currentTime = 0;
		}
	}
}

void Renderer::RenderScene()	
{
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawSkybox();

	//DrawShadowScene();
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	

	BindShader(heightMap->GetShader());
	UpdateShaderMatrices();
	SetShaderLight(*light);
	
	glUniform3fv(glGetUniformLocation(heightMap->GetShader()->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());

	heightMap->Draw();


	DrawNode(root);
}

void Renderer::DrawNode(SceneNode* n) {
	Shader* s = n->GetShader();
	if (s && n->GetMesh())
	{
		if (s != GetCurrentShader())
		{
			BindShader(s);
		}
		UpdateShaderMatrices();
		SetShaderLight(*light);
		Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv( glGetUniformLocation(s->GetProgram(), "modelMatrix"), 1, false, model.values );
		n->Draw(*this);
	}

	for (vector <SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i)
	{
		DrawNode(*i);
	}
}

void Renderer::DrawSkybox()
{
	glDepthMask(GL_FALSE);

	BindShader(cubemap_shader);
	UpdateShaderMatrices();

	basicQuad->Draw();
	glDepthMask(GL_TRUE);
}

void Renderer::LoadShaders()
{
	world_shader = new Shader("WorldVertex.glsl", "WorldFragment.glsl");
	//world_shader = new Shader("ShadowedWorldVert.glsl", "ShadowedWorldFrag.glsl");  // Unused shadow code shader
	cubemap_shader = new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");
	skinning_shader = new Shader("SkinningVertex.glsl", "SkinningFragment.glsl");
	object_shader = new Shader("ObjectVertex.glsl", "ObjectFragment.glsl");
	water_shader = new Shader("reflectVertex.glsl", "reflectFragment.glsl");
	shadow_shader = new Shader("shadowVert.glsl", "shadowFrag.glsl");
}

void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	BindShader(shadow_shader);

	viewMatrix = Matrix4::BuildViewMatrix(heightMap->GetHeightmapSize() * 0.5 + light->GetDirection() * 5000, heightMap->GetHeightmapSize() * 0.5);
	projMatrix = Matrix4::Perspective(1, 10000, 1, 45);
	shadowMatrix = projMatrix * viewMatrix; //used later

	UpdateShaderMatrices();

	heightMap->Draw();

	DrawNodeMesh(root, shadow_shader);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//Only used by shadow code
void Renderer::DrawNodeMesh(SceneNode* n, Shader* s)
{
	if (n->GetMesh())
	{
		Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(s->GetProgram(), "modelMatrix"), 1, false, model.values);
		n->GetMesh()->Draw();
	}

	for (vector <SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i)
	{
		DrawNodeMesh(*i, s);
	}
}

void Renderer::ManageInputs()
{
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_V))
	{
		spin_camera = !spin_camera;
	}
	else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P))
	{
		std::cout << "Camera Position: " << camera->GetPosition();
		std::cout << "Camera Pitch: " << camera->GetPitch() << std::endl;
		std::cout << "Camera Yaw: " << camera->GetYaw() << std::endl;
	}
}