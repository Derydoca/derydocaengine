#pragma once
#include "Mesh.h"
#include "Vertex.h"
#include "SOIL.h"

class Terrain
{
public:
	Terrain(const std::string& fileName, float unitScale, float heightScale);
	Terrain(int width, int depth, float unitScale, float heightScale);
	~Terrain();

	void draw();
	void updateMesh();
private:
	int m_width;
	int m_depth;
	float m_unitScale;
	float m_heightScale;
	float** m_heightData;
	Mesh* m_mesh;
};

