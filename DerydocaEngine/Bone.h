#pragma once

#include <glm/mat4x4.hpp>
#include <string>
#include <vector>

namespace DerydocaEngine::Animation {

	class Bone {
	public:
		Bone();
		Bone(unsigned int id, std::string name, glm::mat4 offset);
		
		unsigned int getTotalBoneCount();
		unsigned int getID() const { return m_id; }
		std::string getName() const { return m_name; }
		size_t getNumChildren() const { return m_children.size(); }
		glm::mat4 getOffset() const { return m_offset; }

		void setName(const std::string& name) { m_name = name; }
		void setOffset(const glm::mat4 offset) { m_offset = offset; }
		void setChildBones(const std::vector<Bone> childBones) { m_children = childBones; }

		const Bone& operator[](unsigned int i) const { return m_children[i]; }
		Bone& operator[](unsigned int i) { return m_children[i]; }
	private:
		unsigned int m_id;
		std::string m_name;
		glm::mat4 m_offset;
		std::vector<Bone> m_children;

		void updateChildCount(unsigned int& boneCount);
	};

}
