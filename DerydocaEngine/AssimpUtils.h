#pragma once
#include <functional>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

#include "Skeleton.h"

namespace DerydocaEngine::Helpers::AssimpUtils
{

	glm::mat4 aiMatToGlm(const aiMatrix4x4& aiMatrix);

	aiBone* findBone(const aiMesh*& mesh, const aiString& name);

	aiNode* findNode(aiNode* node, const aiString& name);

	std::shared_ptr<Animation::Skeleton> getSkeleton(const aiScene*& scene, unsigned int meshIndex);

	void scanNodeChildren(aiNode*& node, std::function<void(aiNode*&)> func);

	void scanNodeParents(aiNode*& node, std::function<bool(aiNode*&)> func);

}