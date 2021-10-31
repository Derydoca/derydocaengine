#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "Resources\Serializers\ResourceSerializer.h"
#include "MeshFlags.h"

struct aiMesh;

namespace DerydocaEngine::Resources::Serializers
{

	class MeshResourceSerializer : public ResourceSerializer
	{
	public:
		virtual std::shared_ptr<void> deserializePointer(std::shared_ptr<Resource> resource);
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
		virtual ResourceType getResourceType() { return ResourceType::MeshResourceType; }

	private:

	};

}
