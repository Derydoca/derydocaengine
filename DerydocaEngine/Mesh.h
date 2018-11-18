#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>

#include "VertexBoneWeights.h"
#include "Color.h"
#include "MeshFlags.h"
#include "Skeleton.h"

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
		BoneIndices = 0x128,
		BoneWeights = 0x256,
		All = Positions | Tangents | Bitangents | TexCoords | Normals | Indices | Colors | BoneIndices | BoneWeights
	};

	class Mesh
	{
	public:
		Mesh();
		Mesh(
			unsigned int numVertices,
			unsigned int numIndices,
			std::vector<glm::vec3> positions,
			std::vector<unsigned int> indices,
			std::vector<glm::vec3> normals,
			std::vector<glm::vec2> texCoords,
			std::vector<glm::vec3> tangents,
			std::vector<glm::vec3> bitangents,
			std::vector<Color> colors,
			std::vector<Animation::VertexBoneWeights> boneWeights);
		~Mesh();
		void loadMeshComponentData(
			MeshComponents const& meshComponentFlags,
			unsigned int const& numVertices,
			std::vector<glm::vec3> const& positions = std::vector<glm::vec3>(),
			std::vector<glm::vec3> const& tangents = std::vector<glm::vec3>(),
			std::vector<glm::vec3> const& bitangents = std::vector<glm::vec3>(),
			std::vector<glm::vec2> const& texCoords = std::vector<glm::vec2>(),
			std::vector<glm::vec3> const& normals = std::vector<glm::vec3>(),
			unsigned int const& numIndices = 0,
			std::vector<unsigned int> const& indices = std::vector<unsigned int>(),
			std::vector<Color> const& colors = std::vector<Color>(),
			std::vector<Animation::VertexBoneWeights> boneWeights = std::vector<Animation::VertexBoneWeights>());
		void draw();
		void setFlags(MeshFlags const& flags) { m_flags = flags; }
		unsigned int getVao() const { return m_vertexArrayObject; }
		unsigned int getNumIndices() const { return m_numIndices; }
		std::shared_ptr<Animation::Skeleton> getSkeleton() { return m_skeleton; }
		void setSkeleton(std::shared_ptr<Animation::Skeleton> skeleton) { m_skeleton = skeleton; }

	private:
		enum {
			POSITION_VB,
			TANGENT_VB,
			BITANGENT_VB,
			TEXCOORD_VB,
			NORMAL_VB,
			INDEX_VB,
			COLOR_VB,
			BONE_INDICES_VB,
			BONE_WEIGHTS_VB,
			NUM_BUFFERS
		};

		Mesh(Mesh const& other) {}
		void operator=(Mesh const& other) {}

		void RefreshVbo();
		void UpdateVbo(MeshComponents const& meshComponentFlags);

		unsigned int m_vertexArrayObject;
		unsigned int m_vertexArrayBuffers[NUM_BUFFERS];
		unsigned int m_numVertices;
		unsigned int m_numIndices;
		std::vector<glm::vec3> m_positions;
		std::vector<unsigned int> m_indices;
		std::vector<glm::vec3> m_normals;
		std::vector<glm::vec2> m_texCoords;
		std::vector<glm::vec3> m_tangents;
		std::vector<glm::vec3> m_bitangents;
		std::vector<Color> m_colors;
		std::vector<Animation::VertexBoneWeights> m_boneWeights;
		std::shared_ptr<Animation::Skeleton> m_skeleton;
		MeshFlags m_flags{};
	};

}
