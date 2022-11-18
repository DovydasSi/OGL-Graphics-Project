#include "WaterNode.h"

WaterNode::WaterNode(Shader* s, Mesh* m, Camera* cam, GLuint cube, DirLight* l) : SceneNode(s, m)
{
	waterRotate = 0.0f;
	waterCycle = 0.0f;
	waterTexture = SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	camera = cam;
	cubeMap = cube;
	light = l;

	glBindTexture(GL_TEXTURE_2D, waterTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void WaterNode::Update(float dt)
{
	waterRotate += dt * 1.2f;
	waterCycle += dt * 0.2f;

	SceneNode::Update(dt);
}

void WaterNode::Draw(const OGLRenderer& r)
{
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "lightDirection"), 1, (float*)& light->GetDirection());
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "cubeTex"), 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	Matrix4 textureMatrix =	Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) *
					Matrix4::Scale(Vector3(10, 10, 10)) *
					Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));

	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "textureMatrix"), 1, false, textureMatrix.values);
	
	SceneNode::Draw(r);
}