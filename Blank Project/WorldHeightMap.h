#pragma once
#include "..//nclgl/HeightMap.h"
#include "..//nclgl/Shader.h"

class WorldHeightMap : public HeightMap
{
public:
	WorldHeightMap(const std::string name, Shader * s, Light* l);
	~WorldHeightMap();

	void Draw();
	Shader* GetShader() { return shader; }
protected:
	Shader* shader;
	Light* light;

	//Add more textures
	GLuint terrainTex;
	GLuint terBumpTex;
	GLuint grassTex;
	GLuint snowTex;
	GLuint shadowTex;
};