#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>

struct aiMesh;

class Mesh
{
public:
	Mesh();
	Mesh(const std::string& fileName);
	Mesh(const std::string& fileName, int meshIndex);
	Mesh(unsigned int numVertices, glm::vec3* positions, glm::vec3* normals, glm::vec2* texCoords, unsigned int* indices, unsigned int numIndices);
	~Mesh();
	void draw();

private:
	Mesh(const Mesh& other) {}
	void operator=(const Mesh& other) {}

	void RefreshVbo();
	void ProcessAiMesh(aiMesh* mesh, int uvIndex);

	enum {
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	glm::vec3* m_positions;
	glm::vec3* m_normals;
	glm::vec2* m_texCoords;
	unsigned int m_numVertices;
	unsigned int* m_indices;
	unsigned int m_numIndices;
	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
};

