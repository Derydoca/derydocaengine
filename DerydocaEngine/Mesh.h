#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include "MeshFlags.h"
#include "Color.h"

struct aiMesh;

enum MeshComponents {
	None = 0x0,
	Positions = 0x1,
	Tangents = 0x2,
	Bitangents = 0x4,
	TexCoords = 0x8,
	Normals = 0x16,
	Indices = 0x32,
	Colors = 0x64,
	All = Positions | Tangents | Bitangents | TexCoords | Normals | Indices | Colors
};

class Mesh
{
public:
	Mesh();
	~Mesh();
	void loadFromFile(const std::string& fileName);
	void loadFromFile(const std::string& fileName, unsigned int meshIndex);
	void loadMeshComponentDataDEPRECATED(unsigned int numVertices, glm::vec3* positions, glm::vec3* normals, glm::vec2* texCoords, unsigned int* indices, unsigned int numIndices);
	void loadMeshComponentData(
		MeshComponents meshComponentFlags,
		unsigned int numVertices,
		glm::vec3 * positions = 0,
		glm::vec3 * tangents = 0,
		glm::vec3 * bitangents = 0,
		glm::vec2 * texCoords = 0,
		glm::vec3 * normals = 0,
		unsigned int numIndices = 0,
		unsigned int * indices = 0,
		Color * colors = 0);
	void loadVertexColorBuffer(unsigned int numVertices, Color* colorBuffer);
	void draw();
	void setFlags(MeshFlags flags) { m_flags = flags; }
	GLuint getVao() const { return m_vertexArrayObject; }
	unsigned int getNumIndices() const { return m_numIndices; }

private:
	enum {
		POSITION_VB,
		TANGENT_VB,
		BITANGENT_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		COLOR_VB,
		NUM_BUFFERS
	};

	Mesh(const Mesh& other) {}
	void operator=(const Mesh& other) {}

	void RefreshVbo();
	void UpdateVbo(MeshComponents meshComponentFlags);
	void ProcessAiMesh(aiMesh* mesh, int uvIndex);

	glm::vec3* m_positions;
	glm::vec3* m_normals;
	glm::vec3* m_tangents;
	glm::vec3* m_bitangents;
	glm::vec2* m_texCoords;
	Color* m_colors;
	unsigned int m_numVertices;
	unsigned int* m_indices;
	unsigned int m_numIndices;
	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	MeshFlags m_flags{};
};

