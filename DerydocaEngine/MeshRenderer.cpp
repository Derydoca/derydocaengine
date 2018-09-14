#include "MeshRenderer.h"
#include "GameObject.h"
#include "Texture.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "ShaderLibrary.h"

namespace DerydocaEngine::Components
{

	MeshRenderer::MeshRenderer()
	{
	}

	MeshRenderer::MeshRenderer(Rendering::Mesh* const& mesh, Rendering::Material* const& material) :
		m_mesh(mesh),
		m_material(material)
	{
	}

	MeshRenderer::~MeshRenderer()
	{
	}

	void MeshRenderer::deserialize(YAML::Node const& compNode)
	{
		auto material = getResourceObject<Rendering::Material>(compNode, "Material");
		setMaterial(material);

		auto mesh = getResourceObject<Rendering::Mesh>(compNode, "Mesh");
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
			m_meshRendererCamera = (DerydocaEngine::Components::Camera*)ObjectLibrary::getInstance().getComponent(renderTextureCameraId);
			material->setTexture(renderTextureName, m_meshRendererCamera->getRenderTexture());
		}
	}

	void MeshRenderer::init()
	{
	}

	void MeshRenderer::render(Rendering::MatrixStack* const& matrixStack)
	{
		assert(getGameObject());

		m_material->bind();
		m_material->getShader()->updateViaActiveCamera(matrixStack);
		Rendering::LightManager::getInstance().bindLightsToShader(matrixStack, getGameObject()->getTransform(), m_material->getShader());

		m_mesh->draw();

		m_material->unbind();
	}

	void MeshRenderer::renderMesh(Rendering::MatrixStack* const& matrixStack, Rendering::Material* const& material, Projection const& projection, Transform* const& projectionTransform)
	{
		material->bind();
		material->getShader()->update(matrixStack, projection, projectionTransform);
		m_mesh->draw();
	}

}
