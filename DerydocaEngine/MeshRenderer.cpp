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
		uuid renderTextureCameraId = renderTextureSourceNode.as<uuid>();
		Camera* renderTextureCamera = (Camera*)ObjectLibrary::getInstance().getComponent(renderTextureCameraId);
		Shader* renderTextureShader = ShaderLibrary::getInstance().find(".\\engineResources\\shaders\\basicShader");
		Material* renderTextureMaterial = new Material();
		renderTextureMaterial->setShader(renderTextureShader);
		renderTextureMaterial->setTextureSlot(0, renderTextureCamera->getRenderTexture());
		setMaterial(renderTextureMaterial);
	}
}

void MeshRenderer::init()
{
}

void MeshRenderer::render(MatrixStack* matrixStack)
{
	assert(getGameObject());

	m_material->bind();
	m_material->getShader()->update(matrixStack);
	LightManager::getInstance().bindLightsToShader(getGameObject()->getTransform(), m_material->getShader());
	m_mesh->draw();
}
