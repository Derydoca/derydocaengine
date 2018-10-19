#pragma once
#include <functional>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include "assimp\cimport.h"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

#include "AnimationData.h"
#include "Skeleton.h"

namespace DerydocaEngine::Helpers::AssimpUtils
{

	glm::mat4 aiToGlm(const aiMatrix4x4& matrix);

	glm::vec3 aiToGlm(const aiVector3D& vec);

	glm::quat aiToGlm(const aiQuaternion& quat);

	aiBone* findBone(const aiMesh*& mesh, const aiString& name);

	aiNode* findNode(aiNode* node, const aiString& name);

	std::shared_ptr<Animation::Skeleton> getSkeleton(const aiScene*& scene, unsigned int meshIndex);

	void scanNodeChildren(aiNode*& node, std::function<void(aiNode*&)> func);

	void scanNodeParents(aiNode*& node, std::function<bool(aiNode*&)> func);

	std::shared_ptr<Animation::AnimationData> getAnimation(const aiScene *& scene, unsigned int animationIndex);

}