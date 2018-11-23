#include "Mesh.h"

#include <GL/glew.h>

#include "MeshAdjacencyCalculator.h"
#include "Debug\DebugVisualizer.h"

namespace DerydocaEngine::Rendering
{

	Mesh::Mesh(
		const std::vector<glm::vec3>& positions,
		const std::vector<unsigned int>& indices,
		const std::vector<glm::vec3>& normals,
		const std::vector<glm::vec2>& texCoords,
		const std::vector<glm::vec3>& tangents,
		const std::vector<glm::vec3>& bitangents,
		const std::vector<Color>& colors,
		const std::vector<Animation::VertexBoneWeights> boneWeights) :
		m_vertexArrayObject(0),
		m_vertexArrayBuffers(),
		m_positions(positions),
		m_indices(indices),
		m_normals(normals),
		m_texCoords(texCoords),
		m_tangents(tangents),
		m_bitangents(bitangents),
		m_colors(colors),
		m_boneWeights(boneWeights),
		m_skeleton()
	{
		// Zero out all buffer handles
		m_vertexArrayBuffers.fill(0);

		// Generate VAO and VBOs
		generateVao();
		bind();
		generateBuffers();

		// Upload mesh component data to the GPU
		uploadToGpu(MeshComponents::All);
	}

	void Mesh::loadMeshComponentData(
		const MeshComponents& meshComponentFlags,
		const std::vector<glm::vec3>& positions,
		const std::vector<unsigned int>& indices,
		const std::vector<glm::vec3>& normals,
		const std::vector<glm::vec2>& texCoords,
		const std::vector<glm::vec3>& tangents,
		const std::vector<glm::vec3>& bitangents,
		const std::vector<Color>& colors,
		const std::vector<Animation::VertexBoneWeights> boneWeights)
	{
		if (meshComponentFlags & MeshComponents::Positions)
		{
			m_positions = positions;
		}

		if (meshComponentFlags & MeshComponents::Indices)
		{
			m_indices = indices;
		}

		if (meshComponentFlags & MeshComponents::Normals)
		{
			m_normals = normals;
		}

		if (meshComponentFlags & MeshComponents::TexCoords)
		{
			m_texCoords = texCoords;
		}

		if (meshComponentFlags & MeshComponents::Tangents)
		{
			m_tangents = tangents;
		}

		if (meshComponentFlags & MeshComponents::Bitangents)
		{
			m_bitangents = bitangents;
		}

		if (meshComponentFlags & MeshComponents::Colors)
		{
			m_colors = colors;
		}

		if (meshComponentFlags & MeshComponents::BoneWeights)
		{
			m_boneWeights = boneWeights;
		}

		uploadToGpu(meshComponentFlags);
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &m_vertexArrayObject);
	}

	void Mesh::uploadToGpu(const MeshComponents& meshComponentFlags)
	{
		assert(m_vertexArrayObject != 0);

		bind();

		// Initialize the vert positions
		if ((meshComponentFlags & MeshComponents::Positions) && m_positions.size() > 0)
		{
			uploadPositions();
		}

		// Initialize the indices buffer
		if ((meshComponentFlags & MeshComponents::Indices) && m_indices.size() > 0)
		{
			uploadIndices();
		}

		// Initialize the normals buffer
		if ((meshComponentFlags & MeshComponents::Normals) && m_normals.size() > 0)
		{
			uploadNormals();
		}

		// Initialize the texture coordinates buffer
		if ((meshComponentFlags & MeshComponents::TexCoords) && m_texCoords.size() > 0)
		{
			uploadTexCoords();
		}

		// Initialize the tangents buffer
		if ((meshComponentFlags & MeshComponents::Tangents) && m_tangents.size() > 0)
		{
			uploadTangents();
		}

		// Initialize the bitangents buffer
		if ((meshComponentFlags & MeshComponents::Bitangents) && m_bitangents.size() > 0)
		{
			uploadBitangents();
		}

		// Initialize the color buffer
		if ((meshComponentFlags & MeshComponents::Colors) && m_colors.size() > 0)
		{
			uploadColors();
		}

		// Initialize the bone index and weight buffers
		if ((meshComponentFlags & MeshComponents::BoneWeights) && m_boneWeights.size() > 0)
		{
			uploadBoneWeights();
		}

		unbind();
	}

	void Mesh::draw()
	{
		bind();

		GLenum mode = m_flags & MeshFlags::load_adjacent ? GL_TRIANGLES_ADJACENCY : GL_TRIANGLES;
		glDrawElementsBaseVertex(mode, static_cast<int>(getNumIndices()), GL_UNSIGNED_INT, 0, 0);

		unbind();
	}

	void Mesh::uploadPositions()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(glm::vec3), &m_positions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	void Mesh::uploadTexCoords()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_texCoords.size() * sizeof(glm::vec2), &m_texCoords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	void Mesh::uploadNormals()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	void Mesh::uploadTangents()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TANGENT_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_tangents.size() * sizeof(glm::vec3), &m_tangents[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	void Mesh::uploadBitangents()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[BITANGENT_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_bitangents.size() * sizeof(glm::vec3), &m_bitangents[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	void Mesh::uploadIndices()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);
	}

	void Mesh::uploadColors()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[COLOR_VB]);
		glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(Color), &m_colors[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, 0);
	}

	void Mesh::uploadBoneWeights()
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

	void Mesh::bind()
	{
		assert(m_vertexArrayObject != 0);
		glBindVertexArray(m_vertexArrayObject);
	}

	void Mesh::unbind()
	{
		glBindVertexArray(0);
	}

	void Mesh::generateVao()
	{
		assert(m_vertexArrayObject == 0);
		glGenVertexArrays(1, &m_vertexArrayObject);
	}

	void Mesh::generateBuffers()
	{
		glGenBuffers(NUM_BUFFERS, &m_vertexArrayBuffers[0]);
	}

}
