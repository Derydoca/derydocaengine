#pragma once
#include "Mesh.h"

class Skybox
{
public:
	Skybox();
	Skybox(float size);
	~Skybox();

	Mesh* getMesh() { return m_mesh; };
private:
	void buildMesh(float size);

	Mesh* m_mesh;
};

