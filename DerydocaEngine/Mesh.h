#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include "MeshFlags.h"

struct aiMesh;

class Mesh
{
public:
	Mesh();
	~Mesh();
	void load(const std::string& fileName);
	void load(const std::string& fileName, unsigned int meshIndex);
	void load(unsigned int numVertices, glm::vec3* positions, glm::vec3* normals, glm::vec2* texCoords, unsigned int* indices, unsigned int numIndices);
	void draw();
	void setFlags(MeshFlags flags) { m_flags = flags; }
	GLuint getVao() const { return m_vertexArrayObject; }
	unsigned int getNumIndices() const { return m_numIndices; }

private:
	Mesh(const Mesh& other) {}
	void operator=(const Mesh& other) {}

	void RefreshVbo();
	void ProcessAiMesh(aiMesh* mesh, int uvIndex);

	enum {
		POSITION_VB,
		TANGENT_VB,
		BITANGENT_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	glm::vec3* m_positions;
	glm::vec3* m_normals;
	glm::vec3* m_tangents;
	glm::vec3* m_bitangents;
	glm::vec2* m_texCoords;
	unsigned int m_numVertices;
	unsigned int* m_indices;
	unsigned int m_numIndices;
	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	MeshFlags m_flags{};
};

