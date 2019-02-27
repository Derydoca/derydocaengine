#include "EngineComponentsPch.h"
#include "Components\MeshRenderer.h"

#include "Components\Camera.h"
#include "Rendering\CameraManager.h"
#include "GameObject.h"
#include "Rendering\LightManager.h"
#include "Rendering\Material.h"
#include "Rendering\Mesh.h"
#include "Rendering\RenderTexture.h"
#include "Rendering\Shader.h"
#include "Rendering\ShaderLibrary.h"
#include "Rendering\Texture.h"
#include "Components\Transform.h"

namespace DerydocaEngine::Components
{

	MeshRenderer::MeshRenderer() :
		m_mesh(),
		m_material(),
		m_meshRendererCamera()
	{
	}

	MeshRenderer::MeshRenderer(std::shared_ptr<Resources::MeshResource> mesh, std::shared_ptr<Resources::MaterialResource> material) :
		m_mesh(mesh),
		m_material(material),
		m_meshRendererCamera()
	{
	}

	MeshRenderer::~MeshRenderer()
	{
	}

	void MeshRenderer::deserialize(const YAML::Node& compNode)
	{
		auto material = getResource<Resources::MaterialResource>(compNode, "Material");
		setMaterial(material);

		auto mesh = getResource<Resources::MeshResource>(compNode, "Mesh");
		setMesh(mesh);

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
			m_meshRendererCamera = ObjectLibrary::getInstance().getComponent<Camera>(renderTextureCameraId);
			getMaterial()->setTexture(renderTextureName, m_meshRendererCamera->getRenderTexture());
		}
	}

	void MeshRenderer::init()
	{
	}

	void MeshRenderer::preDestroy()
	{
		m_meshRendererCamera = nullptr;
	}

	void MeshRenderer::render(std::shared_ptr<Rendering::MatrixStack> const matrixStack)
	{
		auto mesh = getMesh();
		auto material = getMaterial();

		if (!mesh || !material)
		{
			return;
		}

		material->bind();
		material->getShader()->updateViaActiveCamera(matrixStack);
		Rendering::LightManager::getInstance().bindLightsToShader(matrixStack, getGameObject()->getTransform(), material->getShader());
		
		mesh->draw();
		
		material->unbind();
	}

	void MeshRenderer::renderMesh(
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
