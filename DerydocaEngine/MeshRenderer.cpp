#include "MeshRenderer.h"
#include "GameObject.h"
#include "Texture.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "ShaderLibrary.h"

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material) :
	m_mesh(mesh),
	m_material(material)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::deserialize(YAML::Node compNode)
{
	auto material = loadResource<Material*>(compNode, "Material");
	setMaterial(material);

	auto mesh = loadResource<Mesh*>(compNode, "Mesh");
	setMesh(mesh);

	YAML::Node renderTextureSourceNode = compNode["RenderTextureSource"];
	if (renderTextureSourceNode && renderTextureSourceNode.IsScalar())
	{
		// Get the name that should be used to bind this texture to the shader
		string renderTextureName = "RenderTexture";
		YAML::Node renderTextureNameNode = compNode["RenderTextureName"];
		if (renderTextureNameNode != nullptr && renderTextureNameNode.IsScalar())
		{
			renderTextureName = renderTextureNameNode.as<string>();
		}

		uuid renderTextureCameraId = renderTextureSourceNode.as<uuid>();
		m_meshRendererCamera = (Camera*)ObjectLibrary::getInstance().getComponent(renderTextureCameraId);
		material->setTexture(renderTextureName, m_meshRendererCamera->getRenderTexture());
	}
}

void MeshRenderer::init()
{
}

void MeshRenderer::render(MatrixStack* matrixStack)
{
	assert(getGameObject());

	m_material->bind();
	m_material->getShader()->updateViaActiveCamera(matrixStack);
	LightManager::getInstance().bindLightsToShader(matrixStack, getGameObject()->getTransform(), m_material->getShader());

	m_mesh->draw();
}

void MeshRenderer::renderMesh(MatrixStack* matrixStack, Material* material, Projection projection, Transform* projectionTransform)
{
	material->bind();
	material->getShader()->update(matrixStack, projection, projectionTransform);
	m_mesh->draw();
}