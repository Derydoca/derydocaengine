#include "Skybox.h"



Skybox::Skybox()
{
	buildMesh(100.0f);
}


Skybox::~Skybox()
{
}

void Skybox::buildMesh(float size)
{
	Vertex* verts = new Vertex[8]();
	verts[0].setPos(-size,   size,  size);
	verts[1].setPos( size,   size,  size);
	verts[2].setPos( size,   size, -size);
	verts[3].setPos(-size,   size, -size);
	verts[4].setPos(-size,  -size,  size);
	verts[5].setPos( size,  -size,  size);
	verts[6].setPos( size,  -size, -size);
	verts[7].setPos(-size,  -size, -size);

	unsigned int indices[36] = {
		// Top
		2, 1, 0,
		3, 2, 0,
		
		// Right
		1, 2, 5,
		5, 2, 6,

		// Bottom
		4, 5, 6,
		4, 6, 7,

		// Left
		0, 4, 7,
		3, 0, 7,

		// Front
		0, 1, 4,
		1, 5, 4,

		// Back
		2, 3, 6,
		3, 7, 6
	};

	Mesh* m = new Mesh(
		verts, 
		(unsigned int)8, 
		indices, 
		(unsigned int)36);
	m_mesh = m;
}
