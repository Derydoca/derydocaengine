#include "EngineComponentsPch.h"
#include "SkinnedMeshRenderer.h"

#include "Components\Camera.h"
#include "Rendering\CameraManager.h"
#include "GameObject.h"
#include "Rendering\LightManager.h"
#include "Rendering\Material.h"
#include "Rendering\Mesh.h"
#include "Rendering\RenderTexture.h"
#include "Rendering\Shader.h"
#include "Rendering\ShaderLibrary.h"
#include "Resources\Serializers\SkeletonResourceSerializer.h"
#include "Rendering\Texture.h"
#include "Components\Transform.h"

namespace DerydocaEngine::Components
{

	SkinnedMeshRenderer::SkinnedMeshRenderer() :
		m_time(0.0f),
		m_dirty(true),
		m_animation(),
		m_material(),
		m_mesh(),
		m_boneMatrices()
	{
	}

	SkinnedMeshRenderer::~SkinnedMeshRenderer()
	{
	}

	void SkinnedMeshRenderer::deserialize(const YAML::Node& compNode)
	{
		m_animation = getResource<Resources::AnimationResource>(compNode, "Animation");
		m_material = getResource<Resources::MaterialResource>(compNode, "Material");
		m_mesh = getResource<Resources::MeshResource>(compNode, "Mesh");
	}

	void SkinnedMeshRenderer::init()
	{
		if (!isFullyConfigured())
		{
			return;
		}

		fixDirtyStatus();
	}

	void SkinnedMeshRenderer::render(std::shared_ptr<Rendering::MatrixStack> const matrixStack)
	{
		if (!isFullyConfigured())
		{
			return;
		}
		else if (m_dirty)
		{
			fixDirtyStatus();
		}

		getMaterial()->bind();
		getMaterial()->getShader()->updateViaActiveCamera(matrixStack);
		getAnimation()->loadPose(m_time, m_boneMatrices, getMesh()->getSkeleton());
		getMaterial()->setMat4Array("BoneMatrices", m_boneMatrices);

		Rendering::LightManager::getInstance().bindLightsToShader(getGameObject()->getTransform(), getMaterial()->getShader());

		getMesh()->draw();

		getMaterial()->unbind();
	}

	void SkinnedMeshRenderer::renderMesh(
		const std::shared_ptr<Rendering::MatrixStack> matrixStack,
		std::shared_ptr<Rendering::Material> material,
		const Rendering::Projection& projection,
		const std::shared_ptr<Transform> projectionTransform
	)
	{
		if (!isFullyConfigured())
		{
			return;
		}

		material->bind();
		material->getShader()->update(matrixStack, projection, projectionTransform);
		getMesh()->draw();
	}

	void SkinnedMeshRenderer::update(const float deltaTime)
	{
		m_time += deltaTime;
	}

	void SkinnedMeshRenderer::fixDirtyStatus()
	{
		assert(isFullyConfigured());

		m_boneMatrices.resize(getMesh()->getSkeleton()->getNumBones());
		getAnimation()->optimizeForSkeleton(getMesh()->getSkeleton());

		m_dirty = false;
	}

}
