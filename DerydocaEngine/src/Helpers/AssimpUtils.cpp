#include "EnginePch.h"
#include "Helpers\AssimpUtils.h"

namespace DerydocaEngine::Helpers::AssimpUtils {

	bool isBoneInMap(const std::map<std::string, bool>& skeletonMap, std::string& name)
	{
		auto it = skeletonMap.find(name);
		if (it == skeletonMap.end())
		{
			return false;
		}
		return (*it).second;
	}

	glm::mat4 aiToGlm(const aiMatrix4x4 & matrix)
	{
		return glm::transpose(glm::make_mat4(&matrix.a1));
	}

	glm::vec3 aiToGlm(const aiVector3D & vec)
	{
		return glm::vec3(vec.x, vec.y, vec.z);
	}

	glm::quat aiToGlm(const aiQuaternion & quat)
	{
		return glm::quat(quat.w, quat.x, quat.y, quat.z);
	}

	aiNode* findNode(aiNode* node, const aiString& name)
	{
		aiNode* n = nullptr;

		// Try to find the node in the current node's list of children
		n = node->FindNode(name);
		if (n != nullptr)
		{
			return n;
		}

		// If no node was found, recursively search down
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			n = findNode(node, name);
			if (n != nullptr)
			{
				return n;
			}
		}

		return n;
	}

	void scanNodeChildren(aiNode *& node, std::function<void(aiNode*&)> func)
	{
		func(node);
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			aiNode* n = node->mChildren[i];
			scanNodeChildren(n, func);
		}
	}

	void scanNodeParents(aiNode *& node, std::function<bool(aiNode*&)> func)
	{
		if (func(node) && node->mParent)
		{
			aiNode* n = node->mParent;
			scanNodeParents(n, func);
		}
	}

}
