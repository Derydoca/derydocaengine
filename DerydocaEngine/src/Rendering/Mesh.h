#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>

#include "Animation\VertexBoneWeights.h"
#include "Color.h"
#include "MeshFlags.h"
#include "Animation\Skeleton.h"

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
		BoneWeights = 0x128,
		All = Positions | Tangents | Bitangents | TexCoords | Normals | Indices | Colors | BoneWeights
	};

	class Mesh : public Object
	{
	public:
		Mesh(
			const std::vector<glm::vec3>& positions = std::vector<glm::vec3>(),
			const std::vector<unsigned int>& indices = std::vector<unsigned int>(),
			const std::vector<glm::vec3>& normals = std::vector<glm::vec3>(),
			const std::vector<glm::vec2>& texCoords = std::vector<glm::vec2>(),
			const std::vector<glm::vec3>& tangents = std::vector<glm::vec3>(),
			const std::vector<glm::vec3>& bitangents = std::vector<glm::vec3>(),
			const std::vector<Color>& colors = std::vector<Color>(),
			const std::vector<Animation::VertexBoneWeights> boneWeights = std::vector<Animation::VertexBoneWeights>());

		~Mesh();

		virtual unsigned long getTypeId() const { return 0; }

		void loadMeshComponentData(
			const MeshComponents& meshComponentFlags,
			const std::vector<glm::vec3>& positions = std::vector<glm::vec3>(),
			const std::vector<unsigned int>& indices = std::vector<unsigned int>(),
			const std::vector<glm::vec3>& normals = std::vector<glm::vec3>(),
			const std::vector<glm::vec2>& texCoords = std::vector<glm::vec2>(),
			const std::vector<glm::vec3>& tangents = std::vector<glm::vec3>(),
			const std::vector<glm::vec3>& bitangents = std::vector<glm::vec3>(),
			const std::vector<Color>& colors = std::vector<Color>(),
			const std::vector<Animation::VertexBoneWeights> boneWeights = std::vector<Animation::VertexBoneWeights>());
		void draw();
		void setFlags(const MeshFlags& flags) { m_flags = flags; }
		unsigned int getVao() const { return m_vertexArrayObject; }
		size_t getNumVertices() const { return m_positions.size(); }
		size_t getNumIndices() const { return m_indices.size(); }
		std::shared_ptr<Animation::Skeleton> getSkeleton() { return m_skeleton; }
		void setSkeleton(const std::shared_ptr<Animation::Skeleton> skeleton) { m_skeleton = skeleton; }

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

		void uploadToGpu(MeshComponents const& meshComponentFlags);
		void uploadPositions();
		void uploadTexCoords();
		void uploadNormals();
		void uploadTangents();
		void uploadBitangents();
		void uploadIndices();
		void uploadColors();
		void uploadBoneWeights();
		void bind();
		void unbind();
		void generateVao();
		void generateBuffers();

		unsigned int m_vertexArrayObject;
		std::array<unsigned int, NUM_BUFFERS> m_vertexArrayBuffers;
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
