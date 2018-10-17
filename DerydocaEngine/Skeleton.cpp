#include "Skeleton.h"

namespace DerydocaEngine::Animation {
	Skeleton::Skeleton() :
		m_rootTransform(),
		m_rootBone()
	{
	}

	Skeleton::Skeleton(const Bone& rootBone, const glm::mat4& rootTransform) :
		m_rootTransform(rootTransform),
		m_rootBone(std::make_shared<Bone>(rootBone))
	{
		RebuildBoneMaps();
	}

	const Bone* Skeleton::getBone(const unsigned int boneId) {
		return m_boneIdMap[boneId];
	}

	const Bone* Skeleton::getBone(const std::string & boneName)
	{
		return getBone(getBoneID(boneName));
	}

	unsigned int Skeleton::getBoneID(const std::string& boneName)
	{
		// Find the element
		auto it = m_boneNameToIDMap.find(boneName);

		// If it exists, return the bone ID
		if (it != m_boneNameToIDMap.end())
		{
			return it->second;
		}

		// If the ID was not found, return -1 which will resolve to max unsigned int value
		return -1;
	}

	void Skeleton::AddBonesToMap(const Bone& bone)
	{
		m_boneNameToIDMap[bone.getName()] = bone.getID();
		for (unsigned int i = 0; i < bone.getNumChildren(); i++)
		{
			AddBonesToMap(bone[i]);
		}
	}

	void Skeleton::AddBonesToIdMap(const Bone & bone)
	{
		m_boneIdMap[bone.getID()] = &bone;
		for (unsigned int i = 0; i < bone.getNumChildren(); i++)
		{
			AddBonesToIdMap(bone[i]);
		}
	}

	void Skeleton::RebuildBoneMaps()
	{
		// Load the bone index map
		m_boneNameToIDMap.clear();
		AddBonesToMap(*m_rootBone);

		// Load the bone ID map
		m_boneIdMap.clear();
		AddBonesToIdMap(*m_rootBone);
	}

}
