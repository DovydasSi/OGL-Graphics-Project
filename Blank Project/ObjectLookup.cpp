#include "ObjectLookup.h"

void getRedDragonInfo(Mesh** m, MeshAnimation** a, MeshMaterial** mmat)
{
	(*m) = Mesh::LoadFromMeshFile("Dragon_Red.msh");
	(*a) = new MeshAnimation("Dragon_Red_fly_forward.anm");
	(*mmat) = new MeshMaterial("Dragon_Red.mat");
}

void getTowerInfo(Mesh** m, MeshMaterial** mmat)
{
	(*m) = Mesh::LoadFromMeshFile("Tower.msh");
	(*mmat) = new MeshMaterial("Tower.mat");
}