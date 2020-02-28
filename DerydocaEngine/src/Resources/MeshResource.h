#pragma once
#include <boost\uuid\uuid.hpp>
#include "Resources\Resource.h"
#include "MeshFlags.h"

namespace DerydocaEngine::Resources
{

	struct MeshResource : public Resource
	{
	public:
		REGISTER_TYPE_ID(MeshResource);

		MeshResource() :
			Resource(ResourceType::MeshResourceType),
			m_skeletonId(),
			m_meshIndex(0),
			m_meshName(),
			m_flags()
		{}

		void setMeshIndex(unsigned int const& meshIndex) { m_meshIndex = meshIndex; }
		void setMeshName(std::string const& meshName) { m_meshName = meshName; }
		void setSkeletonId(boost::uuids::uuid skeletonId) { m_skeletonId = skeletonId; }

		unsigned int getMeshIndex() { return m_meshIndex; }
		std::string getMeshName() { return m_meshName; }
		Rendering::MeshFlags getFlags() { return m_flags; }
		void setFlag(Rendering::MeshFlags const& flag) {
			m_flags = (Rendering::MeshFlags)(m_flags | flag);
		}
		const boost::uuids::uuid getSkeletonId() const { return m_skeletonId; }
		const bool hasSkeleton() const { return !m_skeletonId.is_nil(); }

		SERIALIZE_FUNC_BASE(
			DerydocaEngine::Resources::Resource,
			SERIALIZE(m_meshIndex),
			SERIALIZE(m_meshName),
			SERIALIZE(m_skeletonId),
			SERIALIZE(m_flags)
		);

	private:
		boost::uuids::uuid m_skeletonId;
		unsigned int m_meshIndex;
		std::string m_meshName;
		Rendering::MeshFlags m_flags{};
	};

}

CEREAL_REGISTER_TYPE(DerydocaEngine::Resources::MeshResource);
