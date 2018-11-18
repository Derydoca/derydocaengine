#include "Mesh.h"

#include <GL/glew.h>

#include "MeshAdjacencyCalculator.h"
#include "GLError.h"

namespace DerydocaEngine::Rendering
{

	Mesh::Mesh() {

	}

	Mesh::Mesh(
		unsigned int numVertices,
		unsigned int numIndices,
		std::vector<glm::vec3> positions,
		std::vector<unsigned int> indices,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texCoords,
		std::vector<glm::vec3> tangents,
		std::vector<glm::vec3> bitangents,
		std::vector<Color> colors,
		std::vector<Animation::VertexBoneWeights> boneWeights) :
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
		std::vector<glm::vec3> const& positions,
		std::vector<glm::vec3> const& tangents,
		std::vector<glm::vec3> const& bitangents,
		std::vector<glm::vec2> const& texCoords,
		std::vector<glm::vec3> const& normals,
		unsigned int const& numIndices,
		std::vector<unsigned int> const& indices,
		std::vector<Color> const& colors,
		std::vector<Animation::VertexBoneWeights> boneWeights)
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
			m_bitangents = bitangents;
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

		if (meshComponentFlags & MeshComponents::BoneWeights)
		{
			m_boneWeights = boneWeights;
		}

		RefreshVbo();
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &m_vertexArrayObject);
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
		if (m_positions.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), &m_positions[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Initialize the texture coordinates buffer
		if (m_texCoords.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec2), &m_texCoords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Initialize the normals buffer
		if (m_normals.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Initialize the tangents buffer
		if (m_tangents.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TANGENT_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), &m_tangents[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Initialize the bitangents buffer
		if (m_bitangents.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[BITANGENT_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), &m_bitangents[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Initialize the indices buffer
		if (m_indices.size() > 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);
		}

		// Initialize the color buffer
		if (m_colors.size() > 0)
		{
			// Upload the buffer to the GPU
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[COLOR_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Color), &m_colors[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Initialize the bone index and weight buffers
		if (m_boneWeights.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[BONE_INDICES_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_boneWeights.size() * sizeof(Animation::VertexBoneWeights), &m_boneWeights[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(6);
			glVertexAttribIPointer(6, Animation::MAX_BONES, GL_UNSIGNED_INT, sizeof(Animation::VertexBoneWeights), 0);

			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[BONE_WEIGHTS_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_boneWeights.size() * sizeof(Animation::VertexBoneWeights), &m_boneWeights[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, Animation::MAX_BONES, GL_FLOAT, GL_FALSE, sizeof(Animation::VertexBoneWeights), (void*)(sizeof(unsigned int) * Animation::MAX_BONES));
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
		if ((meshComponentFlags & MeshComponents::Positions) && m_positions.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), &m_positions[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Initialize the texture coordinates buffer
		if ((meshComponentFlags & MeshComponents::TexCoords) && m_texCoords.size())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec2), &m_texCoords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Initialize the normals buffer
		if ((meshComponentFlags & MeshComponents::Normals) && m_normals.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Initialize the tangents buffer
		if ((meshComponentFlags & MeshComponents::Tangents) && m_tangents.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TANGENT_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), &m_tangents[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Initialize the bitangents buffer
		if ((meshComponentFlags & MeshComponents::Bitangents) && m_bitangents.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[BITANGENT_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(glm::vec3), &m_bitangents[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Initialize the indices buffer
		if ((meshComponentFlags & MeshComponents::Indices) && m_indices.size() > 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);
		}

		// Initialize the color buffer
		if ((meshComponentFlags & MeshComponents::Colors) && m_colors.size() > 0)
		{
			// Upload the buffer to the GPU
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[COLOR_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Color), &m_colors[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Initialize the bone index and weight buffers
		if ((meshComponentFlags & (MeshComponents::BoneIndices | MeshComponents::BoneWeights)) && m_boneWeights.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[BONE_INDICES_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_boneWeights.size() * sizeof(Animation::VertexBoneWeights), &m_boneWeights[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(6);
			glVertexAttribIPointer(6, Animation::MAX_BONES, GL_UNSIGNED_INT, sizeof(Animation::VertexBoneWeights), 0);

			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[BONE_WEIGHTS_VB]);
			glBufferData(GL_ARRAY_BUFFER, m_boneWeights.size() * sizeof(Animation::VertexBoneWeights), &m_boneWeights[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, Animation::MAX_BONES, GL_FLOAT, GL_FALSE, sizeof(Animation::VertexBoneWeights), (void*)(sizeof(unsigned int) * Animation::MAX_BONES));
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
