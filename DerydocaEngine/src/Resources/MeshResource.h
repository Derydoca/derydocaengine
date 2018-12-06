#pragma once
#include <boost\uuid\uuid.hpp>
#include "Resources\Resource.h"
#include "MeshFlags.h"

namespace DerydocaEngine::Resources
{

	struct MeshResource : public Resource
	{
	public:
		MeshResource() :
			m_hasSkeleton(false),
			m_skeletonId(),
			m_meshIndex(0),
			m_meshName(),
			m_flags()
		{}

		void setMeshIndex(unsigned int const& meshIndex) { m_meshIndex = meshIndex; }
		void setMeshName(std::string const& meshName) { m_meshName = meshName; }
		void setSkeletonId(boost::uuids::uuid skeletonId) { m_skeletonId = skeletonId; m_hasSkeleton = true; }

		unsigned int getMeshIndex() { return m_meshIndex; }
		std::string getMeshName() { return m_meshName; }
		Rendering::MeshFlags getFlags() { return m_flags; }
		void setFlag(Rendering::MeshFlags const& flag) {
			m_flags = (Rendering::MeshFlags)(m_flags | flag);
		}
		const boost::uuids::uuid getSkeletonId() const { return m_skeletonId; }
		bool hasSkeleton() const { return m_hasSkeleton; }

	private:
		bool m_hasSkeleton;
		boost::uuids::uuid m_skeletonId;
		unsigned int m_meshIndex;
		std::string m_meshName;
		Rendering::MeshFlags m_flags{};
	};

}
