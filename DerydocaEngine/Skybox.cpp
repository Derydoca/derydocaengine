#include "Skybox.h"



Skybox::Skybox()
{
}


Skybox::~Skybox()
{
}

void Skybox::buildMesh()
{
	Vertex* verts = new Vertex[8]();
	verts[0].setPos(-1,  1,  1);
	verts[1].setPos( 1,  1,  1);
	verts[2].setPos( 1,  1, -1);
	verts[3].setPos(-1,  1, -1);
	verts[4].setPos(-1,  -1,  1);
	verts[5].setPos( 1,  -1,  1);
	verts[6].setPos( 1,  -1, -1);
	verts[7].setPos(-1,  -1, -1);

	verts[0].setTexCoord(0, 1);
	verts[1].setTexCoord(1, 1);
	verts[2].setTexCoord(1, 0);
	verts[3].setTexCoord(0, 0);
	verts[4].setTexCoord(0, 0);
	verts[5].setTexCoord(0, 0);
	verts[6].setTexCoord(0, 0);
	verts[7].setTexCoord(0, 0);

	int indices[36] = {
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
}
