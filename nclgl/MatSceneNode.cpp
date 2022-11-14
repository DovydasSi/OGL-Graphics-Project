#include "MatSceneNode.h"

MatSceneNode::MatSceneNode(Shader* s, Mesh* m, MeshMaterial* mmat) : SceneNode(s, m), meshMat(mmat)
{
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		const MeshMaterialEntry* matEntry = meshMat->GetMaterialForLayer(i);

		const string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		matTextures.emplace_back(texID);
	}
}

MatSceneNode::~MatSceneNode()
{
	delete meshMat;
}

void MatSceneNode::Draw(const OGLRenderer& r)
{
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) 
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, matTextures[i]);
		mesh->DrawSubMesh(i);
	}

	SceneNode::Draw(r);
}