#include "Bone.h"

namespace DerydocaEngine::Animation {
	Bone::Bone() :
		m_id(0),
		m_name(),
		m_offset()
	{
	}

	Bone::Bone(unsigned int id, std::string name, glm::mat4 offset) :
		m_id(id),
		m_name(name),
		m_offset(offset)
	{
	}

	unsigned int Bone::getTotalBoneCount()
	{
		unsigned int count = 0;
		updateChildCount(count);
		return count;
	}

	void Bone::updateChildCount(unsigned int & boneCount)
	{
		boneCount++;
		for (int i = 0; i < m_children.size(); i++)
		{
			m_children[i].updateChildCount(boneCount);
		}
	}

}
