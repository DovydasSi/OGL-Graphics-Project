#include "Renderer.h"
#include "Dragon.h"
#include "ObjectLookup.h"

//Coursework Renderer

Renderer::Renderer(Window &parent) : OGLRenderer(parent), currentTime(0)
{
	light = new DirLight(Vector3(0, 0.5f, 0.5f), Vector4(1, 1, 1, 1), Vector4(0, 1, 0, 1));

	world_shader = new Shader("WorldVertex.glsl", "WorldFragment.glsl");
	heightMap = new WorldHeightMap(TEXTUREDIR"HeightMap.png", world_shader, light);

	camera = new Camera(-40, 70, Vector3());

	Vector3 dimensions = heightMap->GetHeightmapSize();
	camera->SetPosition(dimensions * Vector3(0.5f, 2, 0.5f));
	
	if(!heightMap->GetShader()->LoadSuccess()) {
		return;
	}

	skinning_shader = new Shader("SkinningVertex.glsl", "SkinningFragment.glsl");
	object_shader = new Shader("ObjectVertex.glsl", "ObjectFragment.glsl");

	root = new SceneNode();
	root->SetModelScale(Vector3(2, 2, 2));
	root->SetTransform(Matrix4::Translation(dimensions * Vector3(0.5f, 0.5f, 0.5f)));
	
	Mesh* m; MeshMaterial* mmat; MeshAnimation* a;

	getRedDragonInfo(&m, &a, &mmat);
	Dragon* din_tei = new Dragon(skinning_shader, m, a, mmat);
	din_tei->SetModelScale(root->GetModelScale() * Vector3(1.2f, 1.2f, 1.2f));
	din_tei->SetTransform(Matrix4::Translation(dimensions * Vector3(0.25f, 0.1f, 0.1f)));
	root->AddChild(din_tei);

	getTowerInfo(&m, &mmat);
	SceneNode* tower = new MatSceneNode(object_shader, m, mmat);
	tower->SetModelScale(root->GetModelScale() * Vector3(1.2f, 1.2f, 1.2f));
	root->AddChild(tower);

	/*glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo); //bind
	glUniformBlockBinding(heightMap->GetShader()->GetProgram(), glGetUniformBlockIndex(skinning_shader->GetProgram(), "matrices"), 0); // bind to the "matrices" variable in the shader
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);													// change 0 to 1 if adding another ubo
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(Matrix4), NULL, GL_STATIC_DRAW);						// "3 * sizeof(Matrix4)" bit notes the total size of the UBO, change if adding more stuff there
	*/
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	/*glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(Matrix4), sizeof(Matrix4), &projMatrix.values);		// "2*sizeof(Matrix4)" bit is how much memory to skip to start writing this matrix
																									// "sizeof(Matrix4)" how many bits to write
	glBindBuffer(GL_UNIFORM_BUFFER, 0);//unbind*/

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	init = true;
}

Renderer::~Renderer(void)	{
	delete camera;
	delete heightMap;

	delete world_shader;
	delete skinning_shader;

	delete root;
}

void Renderer::UpdateScene(float dt) 
{
	currentTime += dt;

	camera->UpdateCamera(dt);
	root->Update(dt);
	viewMatrix = camera->BuildViewMatrix();
	
	light->SetDirection( Vector3( 0, sin(currentTime), cos(currentTime) ));

	if (currentTime > 2 * 3.14159)
	{
		currentTime -= 2 * 3.14159;
	}
}

void Renderer::RenderScene()	
{
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	BindShader(heightMap->GetShader());
	UpdateShaderMatrices();
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(heightMap->GetShader()->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());

	heightMap->Draw();

	//BindShader(object_shader);
	//UpdateShaderMatrices();
	DrawNode(root);
}

void Renderer::DrawNode(SceneNode* n) {
	Shader* s = n->GetShader();
	if (s && n->GetMesh())
	{
		BindShader(s);
		UpdateShaderMatrices();
		Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv( glGetUniformLocation(s->GetProgram(), "modelMatrix"), 1, false, model.values );
		n->Draw(*this);
	}

	for (vector <SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i)
	{
		DrawNode(*i);
	}
}