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
		m_animation(),
		m_material(),
		m_mesh(),
		m_SkinnedMeshRendererCamera(),
		m_boneMatrices()
	{
	}

	SkinnedMeshRenderer::~SkinnedMeshRenderer()
	{
	}

	void SkinnedMeshRenderer::deserialize(const YAML::Node& compNode)
	{
		m_material = getResource<Resources::MaterialResource>(compNode, "Material");

		m_mesh = getResource<Resources::MeshResource>(compNode, "Mesh");

		m_animation = getResource<Resources::AnimationResource>(compNode, "Animation");

		YAML::Node renderTextureSourceNode = compNode["RenderTextureSource"];
		if (renderTextureSourceNode && renderTextureSourceNode.IsScalar())
		{
			// Get the name that should be used to bind this texture to the shader
			std::string renderTextureName = "RenderTexture";
			YAML::Node renderTextureNameNode = compNode["RenderTextureName"];
			if (renderTextureNameNode != nullptr && renderTextureNameNode.IsScalar())
			{
				renderTextureName = renderTextureNameNode.as<std::string>();
			}

			boost::uuids::uuid renderTextureCameraId = renderTextureSourceNode.as<boost::uuids::uuid>();
			m_SkinnedMeshRendererCamera = ObjectLibrary::getInstance().getComponent<Camera>(renderTextureCameraId);
			getMaterial()->setTexture(renderTextureName, m_SkinnedMeshRendererCamera->getRenderTexture());
		}
	}

	void SkinnedMeshRenderer::init()
	{
		if (!getMesh())
		{
			return;
		}
		m_boneMatrices.resize(getMesh()->getSkeleton()->getNumBones());
		getAnimation()->optimizeForSkeleton(getMesh()->getSkeleton());
	}

	void SkinnedMeshRenderer::render(std::shared_ptr<Rendering::MatrixStack> const matrixStack)
	{
		if (!getMesh() || !getAnimation() || !getMaterial())
		{
			return;
		}

		getMaterial()->bind();
		getMaterial()->getShader()->updateViaActiveCamera(matrixStack);
		getAnimation()->loadPose(m_time, m_boneMatrices, getMesh()->getSkeleton());
		getMaterial()->setMat4Array("BoneMatrices", m_boneMatrices);

		Rendering::LightManager::getInstance().bindLightsToShader(matrixStack, getGameObject()->getTransform(), getMaterial()->getShader());

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
		material->bind();
		material->getShader()->update(matrixStack, projection, projectionTransform);
		getMesh()->draw();
	}

}
