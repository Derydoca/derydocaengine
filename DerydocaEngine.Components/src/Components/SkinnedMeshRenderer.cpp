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
		m_Time(0.0f),
		m_Dirty(true),
		m_Animation(),
		m_Material(),
		m_Mesh(),
		m_BoneMatrices()
	{
	}

	SkinnedMeshRenderer::~SkinnedMeshRenderer()
	{
	}

	void SkinnedMeshRenderer::deserialize(const YAML::Node& compNode)
	{
		m_Animation.Set(getResource<Resources::AnimationResource>(compNode, "Animation"));
		m_Material.Set(getResource<Resources::MaterialResource>(compNode, "Material"));
		m_Mesh.Set(getResource<Resources::MeshResource>(compNode, "Mesh"));
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
		else if (m_Dirty)
		{
			fixDirtyStatus();
		}

		getMaterial()->bind();
		getMaterial()->getShader()->updateViaActiveCamera(matrixStack);
		getAnimation()->loadPose(m_Time, m_BoneMatrices, getMesh()->getSkeleton());
		getMaterial()->setMat4Array("BoneMatrices", m_BoneMatrices);

		Rendering::LightManager::getInstance().bindLightsToShader(getMaterial()->getShader());

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
		m_Time += deltaTime;
	}

	void SkinnedMeshRenderer::fixDirtyStatus()
	{
		assert(isFullyConfigured());

		m_BoneMatrices.resize(getMesh()->getSkeleton()->getNumBones());
		getAnimation()->optimizeForSkeleton(getMesh()->getSkeleton());

		m_Dirty = false;
	}

	SERIALIZE_FUNC_LOAD(archive, SkinnedMeshRenderer)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_Animation),
			SERIALIZE(m_Material),
			SERIALIZE(m_Mesh)
		);
	}

	SERIALIZE_FUNC_SAVE(archive, SkinnedMeshRenderer)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_Animation),
			SERIALIZE(m_Material),
			SERIALIZE(m_Mesh)
		);
	}

}
