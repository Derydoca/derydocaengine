#include "Mesh.h"

#include <GL/glew.h>

#include "MeshAdjacencyCalculator.h"
#include "GLError.h"

// TODO: Move this some place configurable
int MaxBonesPerVertex = 4;
const int EngineMaxBonesPerVertex = 4;

namespace DerydocaEngine::Rendering
{

	Mesh::Mesh() {

	}

	Mesh::Mesh(
		unsigned int numVertices,
		unsigned int numIndices,
		glm::vec3 * positions,
		unsigned int * indices,
		glm::vec3 * normals,
		glm::vec2 * texCoords,
		glm::vec3 * tangents,
		glm::vec3 * bitangents,
		Color* colors,
		unsigned int* boneIndices,
		float * boneWeights) :
		m_vertexArrayObject(0),
		m_numVertices(numVertices),
		m_numIndices(numIndices),
		m_positions(positions),
		m_indices(indices),
		m_normals(normals),
		m_texCoords(texCoords),
		m_tangents(tangents),
		m_bitangents(bitangents),
		m_colors(colors),
		m_boneIndices(boneIndices),
		m_boneWeights(boneWeights)
	{
		// Initialize the buffer handles
		for (size_t i = 0; i < NUM_BUFFERS; i++)
		{
			m_vertexArrayBuffers[i] = 0;
		}

		// Upload to the GPU
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
		Color * const& colors,
		unsigned int* boneIndices,
		float* boneWeights)
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

		if (meshComponentFlags & MeshComponents::BoneIndices)
		{
			m_boneIndices = boneIndices;
		}

		if (meshComponentFlags & MeshComponents::BoneWeights)
		{
			m_boneWeights = boneWeights;
		}

		RefreshVbo();
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
		delete[] m_boneIndices;
		delete[] m_boneWeights;
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

		// Initialize the bone index buffer
		if (m_boneIndices)
		{
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[BONE_INDICES_VB]));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, m_numVertices * MaxBonesPerVertex * sizeof(unsigned int), m_boneIndices, GL_STATIC_DRAW));
			GL_CHECK(glEnableVertexAttribArray(6));
			GL_CHECK(glVertexAttribIPointer(6, 4, GL_INT, 0, 0));
		}

		// Initialize the bone weight buffer
		if (m_boneWeights)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[BONE_WEIGHTS_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_numVertices * MaxBonesPerVertex * sizeof(float), m_boneWeights, GL_STATIC_DRAW);
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 0, 0);
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

		// Initialize the bone index buffer
		if ((meshComponentFlags & MeshComponents::BoneIndices) && m_boneIndices != nullptr)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[BONE_INDICES_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_numVertices * MaxBonesPerVertex * sizeof(unsigned int), m_boneIndices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(6);
			glVertexAttribIPointer(6, 4, GL_INT, 0, 0);
		}

		// Initialize the bone weight buffer
		if ((meshComponentFlags & MeshComponents::BoneWeights) && m_boneWeights != nullptr)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[BONE_WEIGHTS_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_numVertices * MaxBonesPerVertex * sizeof(float), m_boneWeights, GL_STATIC_DRAW);
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}

		glBindVertexArray(0);
	}

	void Mesh::draw()
	{
		glBindVertexArray(m_vertexArrayObject);

		GLenum mode = m_flags & MeshFlags::load_adjacent ? GL_TRIANGLES_ADJACENCY : GL_TRIANGLES;
		glDrawElementsBaseVertex(mode, m_numIndices, GL_UNSIGNED_INT, 0, 0);

		glBindVertexArray(0);
	}

}
