#include "SceneNode.h"


SceneNode::SceneNode(Shader * s, Mesh* m, Vector4 col)
{
	mesh = m;
	shader = s;
	colour = col;
	parent = NULL;
	modelScale = Vector3(1, 1, 1);

	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
	texture = 0;
}

SceneNode::~SceneNode()
{
	for (unsigned int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}

void SceneNode::AddChild(SceneNode* s)
{
	if (s != this)
	{
		children.push_back(s);
		s->parent = this;
	}
}

void SceneNode::Draw(const OGLRenderer& r)
{
	if (mesh)
	{
		mesh->Draw();
	}
}

void SceneNode::Update(float dt)
{
	if (parent)
	{
		worldTransform = parent->worldTransform * transform;
	}
	else
	{
		worldTransform = transform;
	}

	for (auto i = children.begin(); i != children.end(); i++)
	{
		(*i)->Update(dt);
	}
}