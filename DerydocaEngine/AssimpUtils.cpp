#include "AssimpUtils.h"

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

	std::vector<Animation::Bone> childrenToBones(const aiMesh*& mesh, aiNode*& boneNode, unsigned int& boneIndex, const std::map<std::string, bool>& skeletonMap)
	{
		// Get the number of child bones so that we only allocate once for the bone vector
		unsigned int numBones = 0;
		for (unsigned int i = 0; i < boneNode->mNumChildren; i++)
		{
			std::string nodeName = boneNode->mChildren[i]->mName.data;
			if (isBoneInMap(skeletonMap, nodeName))
			{
				numBones++;
			}
		};

		// Create the vector of bones
		std::vector<Animation::Bone> bones(numBones);

		// Create each individual bone in the vector
		unsigned int childBoneCount = 0;
		for (unsigned int i = 0; i < boneNode->mNumChildren; i++)
		{
			std::string nodeName = boneNode->mChildren[i]->mName.data;
			if (isBoneInMap(skeletonMap, nodeName))
			{
				aiNode* childNode = boneNode->mChildren[i];
				aiBone* b = findBone(mesh, childNode->mName);
				glm::mat4 offset = aiMatToGlm((b == nullptr) ? childNode->mTransformation : b->mOffsetMatrix);
				Animation::Bone childBone(boneIndex++, childNode->mName.data, offset);

				std::vector<Animation::Bone> childBones = childrenToBones(mesh, childNode, boneIndex, skeletonMap);
				childBone.setChildBones(childBones);

				bones[childBoneCount++] = childBone;
			}
		}

		// return the bones
		return bones;
	}

	glm::mat4 aiMatToGlm(const aiMatrix4x4 & aiMatrix)
	{
		return glm::transpose(glm::make_mat4(&aiMatrix.a1));
	}

	aiBone* findBone(const aiMesh*& mesh, const aiString& name)
	{
		// Find the first bone that matches the name provided
		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			if (mesh->mBones[i]->mName == name)
			{
				// Return the found bone
				return mesh->mBones[i];
			}
		}

		// Return a null pointer if no bone was found
		return nullptr;
	}

	aiNode* findNode(aiNode* node, const aiString& name)
	{
		aiNode* n = nullptr;

		n = node->FindNode(name);
		if (n != nullptr)
		{
			return n;
		}

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

	std::shared_ptr<Animation::Skeleton> getSkeleton(const aiScene *& scene, unsigned int meshIndex)
	{
		aiNode* rootNode = scene->mRootNode;

		assert(scene->HasMeshes() && meshIndex < scene->mNumMeshes);

		const aiMesh* mesh = scene->mMeshes[meshIndex];

		assert(mesh->HasBones());

		// Find the node for the specified mesh
		aiNode* meshNode = nullptr;
		scanNodeChildren(rootNode, [meshIndex, &meshNode](aiNode*& node) {
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				if (meshIndex == node->mMeshes[i])
				{
					meshNode = const_cast<aiNode*>(node);
				}
			}

			return meshNode == nullptr;
		});
		assert(meshNode);

		// Get the names of the mesh's node and its parent
		aiString meshNodeName = meshNode->mName;
		aiString parentNodeName;
		if (meshNode->mParent)
		{
			parentNodeName = meshNode->mParent->mName;
		}

		// This map stores a list of node names and a boolean value to mark if it is part of the skeleton
		std::map<std::string, bool> skeletonNodeMap;

		// Prepopulate all nodes to be marked false in the map
		scanNodeChildren(rootNode, [&skeletonNodeMap](aiNode*& node) {
			if (node->mName.length > 0)
			{
				skeletonNodeMap[node->mName.data] = false;
			}
		});

		// Iterate through each bone, find the corresponding node, mark that as included and mark all parents as included
		aiNode* n = nullptr; /* Declaring the node outside of the loop for easy recovery of the root node in the next step*/
		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			// Get the bone
			aiBone* b = mesh->mBones[i];

			// Find the node
			n = findNode(scene->mRootNode, b->mName);
			if (!n)
			{
				continue;
			}

			// Mark this node as included in the skeleton and mark all parent nodes up until we find the root
			scanNodeParents(n, [&skeletonNodeMap, &meshNodeName, &parentNodeName](aiNode*& node) {
				auto name = node->mName;

				bool isValid = name.length > 0 && name != meshNodeName && name != parentNodeName;

				// Mark this node as true
				if (isValid)
				{
					skeletonNodeMap[name.data] = true;
				}

				// Stop traversing if the node we are at is the mesh node or mesh node's parent
				return isValid;
			});
		}

		// Find the skeleton root node
		scanNodeParents(n, [&skeletonNodeMap, &rootNode](aiNode*& node) {
			// If this node is marked as included in the map, record it as the root node. It may not be the root node, but if our next
			//  iteration returns false, it is.
			std::string nodeName = node->mName.data;
			if (isBoneInMap(skeletonNodeMap, nodeName))
			{
				rootNode = node;
			}
			else
			{
				// We found the first node not in the skeleton map so let's exit out
				return false;
			}

			// Keep searching if we are here
			return true;
		});

		// Build the bones of the skeleton
		unsigned int boneIndex = 0;
		Animation::Bone rootBone = Animation::Bone(boneIndex++, rootNode->mName.data, aiMatToGlm(rootNode->mTransformation));
		std::vector<Animation::Bone> childBones = childrenToBones(mesh, rootNode, boneIndex, skeletonNodeMap);
		rootBone.setChildBones(childBones);

		// Create the skeleton object
		std::shared_ptr<Animation::Skeleton> skeleton = std::make_shared<Animation::Skeleton>(Animation::Skeleton(rootBone, glm::mat4()));

		// Return the generated skeleton
		return skeleton;
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
