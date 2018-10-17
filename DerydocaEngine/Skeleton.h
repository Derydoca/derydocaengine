#pragma once
#include <glm/mat4x4.hpp>
#include <map>
#include <vector>
#include "Bone.h"

namespace DerydocaEngine::Animation {

	class Skeleton {
	public:
		Skeleton();
		Skeleton(const Bone& rootBone, const glm::mat4& rootTransform);

		std::string getName() const { return m_rootBone->getName(); }
		size_t getNumBones() const { return  m_rootBone->getTotalBoneCount(); }
		const Bone* getBone(const unsigned int boneIndex);
		const Bone* getBone(const std::string& boneName);
		unsigned int getBoneID(const std::string& boneName);
	private:
		std::map<std::string, unsigned int> m_boneNameToIDMap;
		std::map<unsigned int, const Bone*> m_boneIdMap;
		std::shared_ptr<Bone> m_rootBone;
		glm::mat4 m_rootTransform;

		void AddBonesToMap(const Bone& bone);
		void AddBonesToIdMap(const Bone& bone);
		void RebuildBoneMaps();
	};

}
