#include "Mesh.h"

#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

Mesh::Mesh() {

}

Mesh::Mesh(const std::string& fileName) : Mesh(fileName, 0)
{
}

Mesh::Mesh(const std::string & fileName, unsigned int meshIndex)
{
	const aiScene* aiModel = aiImportFile(fileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	int uvIndex = 0;

	if (meshIndex >= aiModel->mNumMeshes)
	{
		return;
	}

	aiMesh* mesh = aiModel->mMeshes[meshIndex];

	ProcessAiMesh(mesh, uvIndex);

	RefreshVbo();
}

Mesh::Mesh(unsigned int numVertices, glm::vec3* positions, glm::vec3* normals, glm::vec2* texCoords, unsigned int* indices, unsigned int numIndices) :
	m_positions(positions),
	m_numVertices(numVertices),
	m_indices(indices),
	m_numIndices(numIndices),
	m_normals(normals),
	m_texCoords(texCoords)
{
	RefreshVbo();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::RefreshVbo()
{
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

	// Initialize the vert positions
	if (m_positions != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), m_positions, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Initialize the texture coordinates buffer
	if (m_texCoords != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec2), m_texCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Initialize the normals buffer
	if (m_normals != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), m_normals, GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Initialize the indices buffer
	if (m_indices != nullptr)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), m_indices, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

void Mesh::ProcessAiMesh(aiMesh * mesh, int uvIndex)
{

	m_numVertices = mesh->mNumVertices;

	if (mesh->HasPositions())
	{
		m_positions = new glm::vec3[m_numVertices];
		for (unsigned int i = 0; i < m_numVertices; i++)
		{
			m_positions[i] = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		}
	}

	if (mesh->HasTextureCoords(uvIndex))
	{
		m_texCoords = new glm::vec2[m_numVertices];
		for (unsigned int i = 0; i < m_numVertices; i++)
		{
			m_texCoords[i] = glm::vec2(mesh->mTextureCoords[uvIndex][i].x, mesh->mTextureCoords[uvIndex][i].y);
		}
	}

	if (mesh->HasNormals())
	{
		m_normals = new glm::vec3[m_numVertices];
		for (unsigned int i = 0; i < m_numVertices; i++)
		{
			m_normals[i] = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}
	}

	if (mesh->HasFaces())
	{
		m_numIndices = mesh->mNumFaces * 3;
		m_indices = new unsigned int[m_numIndices];
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			m_indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
			m_indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			m_indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}
	}

}

void Mesh::draw()
{
	glBindVertexArray(m_vertexArrayObject);

	glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);

	glBindVertexArray(0);
}
