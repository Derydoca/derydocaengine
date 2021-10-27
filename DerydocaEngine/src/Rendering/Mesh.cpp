#include "EnginePch.h"
#include "Rendering\Mesh.h"

#include "MeshAdjacencyCalculator.h"
#include "GraphicsAPI.h"

namespace DerydocaEngine::Rendering
{

	Mesh::Mesh(
		const std::vector<glm::vec3>& positions,
		const std::vector<unsigned int>& indices,
		const std::vector<glm::vec3>& normals,
		const std::vector<glm::vec2>& texCoords,
		const std::vector<glm::vec3>& tangents,
		const std::vector<glm::vec3>& bitangents,
		const std::vector<Color>& colors) :
		m_vertexArrayObject(0),
		m_vertexArrayBuffers(),
		m_positions(positions),
		m_indices(indices),
		m_normals(normals),
		m_texCoords(texCoords),
		m_tangents(tangents),
		m_bitangents(bitangents),
		m_colors(colors)
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
		const std::vector<Color>& colors)
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
		GraphicsAPI::uploadVertexBuffer(m_vertexArrayBuffers[POSITION_VB], DataType::Float, 3, &m_positions[0], m_positions.size(), 0);
	}

	void Mesh::uploadTexCoords()
	{
		GraphicsAPI::uploadVertexBuffer(m_vertexArrayBuffers[TEXCOORD_VB], DataType::Float, 2, &m_texCoords[0], m_texCoords.size(), 1);
	}

	void Mesh::uploadNormals()
	{
		GraphicsAPI::uploadVertexBuffer(m_vertexArrayBuffers[NORMAL_VB], DataType::Float, 3, &m_normals[0], m_normals.size(), 2);
	}

	void Mesh::uploadTangents()
	{
		GraphicsAPI::uploadVertexBuffer(m_vertexArrayBuffers[TANGENT_VB], DataType::Float, 3, &m_tangents[0], m_tangents.size(), 3);
	}

	void Mesh::uploadBitangents()
	{
		GraphicsAPI::uploadVertexBuffer(m_vertexArrayBuffers[BITANGENT_VB], DataType::Float, 3, &m_bitangents[0], m_bitangents.size(), 4);
	}

	void Mesh::uploadIndices()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);
	}

	void Mesh::uploadColors()
	{
		GraphicsAPI::uploadVertexBuffer(m_vertexArrayBuffers[COLOR_VB], DataType::Float, 4, &m_colors[0], m_colors.size(), 5);
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
