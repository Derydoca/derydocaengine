#pragma once
#include "Mesh.h"

class Skybox
{
public:
	Skybox();
	~Skybox();
private:
	void buildMesh();

	Mesh m_mesh;
};

