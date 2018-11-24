#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "Animation\VertexBoneWeights.h"
#include "Resources\Serializers\ResourceSerializer.h"
#include "MeshFlags.h"
#include "Animation\Skeleton.h"

struct aiMesh;

namespace DerydocaEngine::Resources::Serializers
{

	class MeshResourceSerializer : public ResourceSerializer
	{
	public:
		MeshResourceSerializer();
		~MeshResourceSerializer();

		void* deserialize(Resource* const& resource);
		virtual std::shared_ptr<void> deserializePointer(Resource* const& resource);
		void ProcessMeshData(
			aiMesh * &mesh,
			std::vector<glm::vec3> &m_positions,
			int uvIndex,
			std::vector<glm::vec2> &m_texCoords,
			std::vector<glm::vec3> &m_normals,
			DerydocaEngine::Rendering::MeshFlags m_flags,
			std::vector<unsigned int> &m_indices,
			std::vector<glm::vec3> &m_tangents,
			std::vector<glm::vec3> &m_bitangents);
		void ProcessBoneData(
			aiMesh * mesh,
			std::vector<Animation::VertexBoneWeights> &m_boneWeights,
			const std::shared_ptr<Animation::Skeleton>& skeleton);
		ResourceType getResourceType();

	private:

	};

}
