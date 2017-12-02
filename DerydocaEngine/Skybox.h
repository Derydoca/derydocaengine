#pragma once
#include "Mesh.h"

class Skybox
{
public:
	Skybox();
	~Skybox();

	Mesh* getMesh() { return m_mesh; };
private:
	void buildMesh(float size);

	Mesh* m_mesh;
};

