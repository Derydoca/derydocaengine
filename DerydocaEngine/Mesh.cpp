#include "Mesh.h"

#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include "MeshAdjacencyCalculator.h"

Mesh::Mesh() {

}

void Mesh::loadFromFile(std::string const& fileName)
{
	loadFromFile(fileName, 0);
}

void Mesh::loadFromFile(std::string const& fileName, unsigned int const& meshIndex)
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

void Mesh::loadMeshComponentDataDEPRECATED(unsigned int const& numVertices, glm::vec3* const& positions, glm::vec3* const& normals, glm::vec2* const& texCoords, unsigned int* const& indices, unsigned int const& numIndices)
{
	m_positions = positions;
	m_numVertices = numVertices;
	m_indices = indices;
	m_numIndices = numIndices;
	m_normals = normals;
	m_texCoords = texCoords;

	RefreshVbo();
}

void Mesh::loadMeshComponentData(
	MeshComponents const& meshComponentFlags,
	unsigned int const& numVertices,
	glm::vec3 * const& positions,
	glm::vec3 * const& tangents,
	glm::vec3 * const& bitangents,
	glm::vec2 * const& texCoords,
	glm::vec3 * const& normals,
	unsigned int const& numIndices,
	unsigned int * const& indices,
	Color * const& colors)
{
	m_numVertices = numVertices;
	
	if (meshComponentFlags & MeshComponents::Positions)
	{
		m_positions = positions;
	}

	if (meshComponentFlags & MeshComponents::Tangents)
	{
		m_tangents = tangents;
	}

	if (meshComponentFlags & MeshComponents::Bitangents)
	{
		m_tangents = bitangents;
	}

	if (meshComponentFlags & MeshComponents::TexCoords)
	{
		m_texCoords = texCoords;
	}

	if (meshComponentFlags & MeshComponents::Normals)
	{
		m_normals = normals;
	}

	if (meshComponentFlags & MeshComponents::Indices)
	{
		m_numIndices = numIndices;
		m_indices = indices;
	}

	if (meshComponentFlags & MeshComponents::Colors)
	{
		m_colors = colors;
	}

	UpdateVbo(meshComponentFlags);
}

void Mesh::loadVertexColorBuffer(unsigned int const& numVertices, Color * const& colorBuffer)
{
	glBindVertexArray(m_vertexArrayObject);

	// Ensure that we have generated a buffer for this mesh
	if (!m_vertexArrayBuffers[COLOR_VB])
	{
		glGenBuffers(1, &m_vertexArrayBuffers[COLOR_VB]);
	}

	m_colors = colorBuffer;

	// Upload the buffer to the GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Color), m_colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
	delete[] m_positions;
	delete[] m_texCoords;
	delete[] m_normals;
	delete[] m_tangents;
	delete[] m_indices;
	delete[] m_bitangents;
}

void Mesh::RefreshVbo()
{
	if (!m_vertexArrayObject)
	{
		glGenVertexArrays(1, &m_vertexArrayObject);
	}
	glBindVertexArray(m_vertexArrayObject);

	if (!m_vertexArrayBuffers[0])
	{
		glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	}

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

	// Initialize the tangents buffer
	if (m_tangents != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TANGENT_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), m_tangents, GL_STATIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Initialize the bitangents buffer
	if (m_bitangents != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[BITANGENT_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), m_bitangents, GL_STATIC_DRAW);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Initialize the indices buffer
	if (m_indices != nullptr)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), m_indices, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

void Mesh::UpdateVbo(MeshComponents const& meshComponentFlags)
{
	if (!m_vertexArrayObject)
	{
		glGenVertexArrays(1, &m_vertexArrayObject);
	}
	glBindVertexArray(m_vertexArrayObject);

	if (!m_vertexArrayBuffers[0])
	{
		glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	}

	// Initialize the vert positions
	if ((meshComponentFlags & MeshComponents::Positions) && m_positions != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), m_positions, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Initialize the texture coordinates buffer
	if ((meshComponentFlags & MeshComponents::TexCoords) && m_texCoords != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec2), m_texCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Initialize the normals buffer
	if ((meshComponentFlags & MeshComponents::Normals) && m_normals != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), m_normals, GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Initialize the tangents buffer
	if ((meshComponentFlags & MeshComponents::Tangents) && m_tangents != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TANGENT_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), m_tangents, GL_STATIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Initialize the bitangents buffer
	if ((meshComponentFlags & MeshComponents::Bitangents) && m_bitangents != nullptr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[BITANGENT_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), m_bitangents, GL_STATIC_DRAW);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Initialize the indices buffer
	if ((meshComponentFlags & MeshComponents::Indices) && m_indices != nullptr)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), m_indices, GL_STATIC_DRAW);
	}

	if ((meshComponentFlags & MeshComponents::Colors) && m_colors != nullptr)
	{
		// Upload the buffer to the GPU
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[COLOR_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Color), m_colors, GL_STATIC_DRAW);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindVertexArray(0);
}

void Mesh::ProcessAiMesh(aiMesh * const& mesh, int const& uvIndex)
{

	m_numVertices = mesh->mNumVertices;

	if (mesh->HasPositions())
	{
		delete[] m_positions;
		m_positions = new glm::vec3[m_numVertices];
		for (unsigned int i = 0; i < m_numVertices; i++)
		{
			m_positions[i] = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		}
	}

	if (mesh->HasTextureCoords(uvIndex))
	{
		delete[] m_texCoords;
		m_texCoords = new glm::vec2[m_numVertices];
		for (unsigned int i = 0; i < m_numVertices; i++)
		{
			m_texCoords[i] = glm::vec2(mesh->mTextureCoords[uvIndex][i].x, mesh->mTextureCoords[uvIndex][i].y);
		}
	}

	if (mesh->HasNormals())
	{
		delete[] m_normals;
		m_normals = new glm::vec3[m_numVertices];
		for (unsigned int i = 0; i < m_numVertices; i++)
		{
			m_normals[i] = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}
	}

	if (mesh->HasFaces())
	{
		if (m_flags & MeshFlags::load_adjacent)
		{
			DerydocaEngine::Ext::MeshAdjacencyCalculator mac;
			m_numIndices = mesh->mNumFaces * 3 * 2;
			delete[] m_indices;
			m_indices = new unsigned int[m_numIndices];
			mac.buildAdjacencyList(mesh, m_indices);
		}
		else
		{
			m_numIndices = mesh->mNumFaces * 3;
			delete[] m_indices;
			m_indices = new unsigned int[m_numIndices];
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				m_indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
				m_indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
				m_indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
			}
		}
	}

	if (mesh->HasTangentsAndBitangents())
	{
		delete[] m_tangents;
		m_tangents = new glm::vec3[m_numVertices];
		for (unsigned int i = 0; i < m_numVertices; i++)
		{
			m_tangents[i] = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
		}

		delete[] m_bitangents;
		m_bitangents = new glm::vec3[m_numVertices];
		for (unsigned int i = 0; i < m_numVertices; i++)
		{
			m_bitangents[i] = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}
	}

}

void Mesh::draw()
{
	glBindVertexArray(m_vertexArrayObject);

	GLenum mode = m_flags & MeshFlags::load_adjacent ? GL_TRIANGLES_ADJACENCY : GL_TRIANGLES;
	glDrawElementsBaseVertex(mode, m_numIndices, GL_UNSIGNED_INT, 0, 0);

	glBindVertexArray(0);
}
