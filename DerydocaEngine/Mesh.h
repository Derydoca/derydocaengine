#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include "MeshFlags.h"
#include "Color.h"

struct aiMesh;

namespace DerydocaEngine::Rendering
{

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
		void loadFromFile(std::string const& fileName);
		void loadFromFile(std::string const& fileName, unsigned int const& meshIndex);
		void loadMeshComponentDataDEPRECATED(unsigned int const& numVertices, glm::vec3* const& positions, glm::vec3* const& normals, glm::vec2* const& texCoords, unsigned int* const& indices, unsigned int const& numIndices);
		void loadMeshComponentData(
			MeshComponents const& meshComponentFlags,
			unsigned int const& numVertices,
			glm::vec3 * const& positions = 0,
			glm::vec3 * const& tangents = 0,
			glm::vec3 * const& bitangents = 0,
			glm::vec2 * const& texCoords = 0,
			glm::vec3 * const& normals = 0,
			unsigned int const& numIndices = 0,
			unsigned int * const& indices = 0,
			Color * const& colors = 0);
		void loadVertexColorBuffer(unsigned int const& numVertices, Color* const& colorBuffer);
		void draw();
		void setFlags(MeshFlags const& flags) { m_flags = flags; }
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

		Mesh(Mesh const& other) {}
		void operator=(Mesh const& other) {}

		void RefreshVbo();
		void UpdateVbo(MeshComponents const& meshComponentFlags);
		void ProcessAiMesh(aiMesh* const& mesh, int const& uvIndex);

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

}
