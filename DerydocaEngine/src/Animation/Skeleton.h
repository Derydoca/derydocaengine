#pragma once
#include <glm/mat4x4.hpp>
#include <map>
#include <vector>
#include "Animation\Bone.h"

namespace DerydocaEngine::Animation {

	class Skeleton {
	public:
		Skeleton();
		Skeleton(const std::shared_ptr<Bone>& rootBone, const glm::mat4& rootTransform);
		
		const glm::mat4& getGlobalInverseTransform() const { return m_rootTransform; }
		std::string getName() const { return m_rootBone->getName(); }
		size_t getNumBones() const { return  m_rootBone->getTotalBoneCount(); }
		const std::shared_ptr<Bone> getRootBone() const { return m_rootBone; };
		const std::shared_ptr<Bone> getBone(unsigned int boneIndex) const;
		const std::shared_ptr<Bone> getBone(const std::string& boneName) const;
		unsigned int getBoneID(const std::string& boneName) const;
	private:
		std::map<std::string, unsigned int> m_boneNameToIDMap;
		std::map<unsigned int, std::shared_ptr<Bone>> m_boneIdMap;
		std::shared_ptr<Bone> m_rootBone;
		glm::mat4 m_rootTransform;

		void AddBonesToMap(const std::shared_ptr<Bone>& bone);
		void AddBonesToIdMap(const std::shared_ptr<Bone>& bone);
		void RebuildBoneMaps();
	};

}
