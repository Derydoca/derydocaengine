#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "ResourceSerializer.h"
#include "MeshFlags.h"
#include "Skeleton.h"

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
		void ProcessMeshData(aiMesh * &mesh, glm::vec3 * &m_positions, unsigned int m_numVertices, int uvIndex, glm::vec2 * &m_texCoords, glm::vec3 * &m_normals, DerydocaEngine::Rendering::MeshFlags m_flags, unsigned int &m_numIndices, unsigned int * &m_indices, glm::vec3 * &m_tangents, glm::vec3 * &m_bitangents);
		void ProcessBoneData(aiMesh * mesh, unsigned int * &m_boneIndices, float * &m_boneWeights, const std::shared_ptr<Animation::Skeleton>& skeleton);
		ResourceType getResourceType();

	private:

	};

}
