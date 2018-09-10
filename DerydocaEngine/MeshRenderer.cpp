#include "MeshRenderer.h"
#include "GameObject.h"
#include "Texture.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "ShaderLibrary.h"

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::MeshRenderer(Mesh* const& mesh, Material* const& material) :
	m_mesh(mesh),
	m_material(material)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::deserialize(YAML::Node const& compNode)
{
	auto material = getResourceObject<Material>(compNode, "Material");
	setMaterial(material);

	auto mesh = getResourceObject<Mesh>(compNode, "Mesh");
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
		m_meshRendererCamera = (Camera*)ObjectLibrary::getInstance().getComponent(renderTextureCameraId);
		material->setTexture(renderTextureName, m_meshRendererCamera->getRenderTexture());
	}
}

void MeshRenderer::init()
{
}

void MeshRenderer::render(MatrixStack* const& matrixStack)
{
	assert(getGameObject());

	m_material->bind();
	m_material->getShader()->updateViaActiveCamera(matrixStack);
	LightManager::getInstance().bindLightsToShader(matrixStack, getGameObject()->getTransform(), m_material->getShader());

	m_mesh->draw();

	m_material->unbind();
}

void MeshRenderer::renderMesh(MatrixStack* const& matrixStack, Material* const& material, Projection const& projection, Transform* const& projectionTransform)
{
	material->bind();
	material->getShader()->update(matrixStack, projection, projectionTransform);
	m_mesh->draw();
}