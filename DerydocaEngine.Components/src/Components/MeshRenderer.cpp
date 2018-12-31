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

	MeshRenderer::MeshRenderer(std::shared_ptr<Rendering::Mesh> mesh, std::shared_ptr<Rendering::Material> material) :
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
		auto material = getResourcePointer<Rendering::Material>(compNode, "Material");
		setMaterial(material);

		auto mesh = getResourcePointer<Rendering::Mesh>(compNode, "Mesh");
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
			material->setTexture(renderTextureName, m_meshRendererCamera->getRenderTexture());
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
		m_material->bind();
		m_material->getShader()->updateViaActiveCamera(matrixStack);
		Rendering::LightManager::getInstance().bindLightsToShader(matrixStack, getGameObject()->getTransform(), m_material->getShader());

		m_mesh->draw();

		m_material->unbind();
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
		m_mesh->draw();
	}

}
