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
#include "Scenes\SceneManager.h"

namespace DerydocaEngine::Components
{

	MeshRenderer::MeshRenderer() :
		m_Mesh(),
		m_Material(),
		m_Transparent(false),
		m_FindRenderTextureCameraHack(false)
	{
	}

	MeshRenderer::MeshRenderer(std::shared_ptr<Resources::MeshResource> mesh, std::shared_ptr<Resources::MaterialResource> material) :
		m_Mesh(mesh),
		m_Material(material),
		m_Transparent(false),
		m_FindRenderTextureCameraHack(true)
	{
	}

	MeshRenderer::~MeshRenderer()
	{
	}

	void MeshRenderer::init()
	{
		if (m_FindRenderTextureCameraHack)
		{
			auto camera = Scenes::SceneManager::getInstance().getActiveScene()->getRoot()->getComponentInChildren<Components::Camera>();
			if (camera)
			{
				setRenderTextureSource(camera, "RenderTex");
			}
		}
	}

	void MeshRenderer::preDestroy()
	{
	}

	void MeshRenderer::setRenderTextureSource(const std::shared_ptr<Components::Camera> camera, const std::string& textureName)
	{
		m_MeshRendererCamera = camera;
		getMaterial()->setTexture(textureName, camera->getRenderTexture());
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
		Rendering::LightManager::getInstance().bindLightsToShader(material->getShader());

		if (m_Transparent)
		{
			Rendering::GraphicsAPI::enableTransparancy();
		}
		mesh->draw();
		if (m_Transparent)
		{
			Rendering::GraphicsAPI::disableTransparancy();
		}

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

	SERIALIZE_FUNC_LOAD(archive, MeshRenderer)
	{
		if (version == 0)
		{
			archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
				SERIALIZE(m_Mesh),
				SERIALIZE(m_Material),
				SERIALIZE(m_Transparent)
			);
		}
		else if (version == 1)
		{
			archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
				SERIALIZE(m_Mesh),
				SERIALIZE(m_Material),
				SERIALIZE(m_Transparent),
				SERIALIZE(m_FindRenderTextureCameraHack)
			);
		}
	}

	SERIALIZE_FUNC_SAVE(archive, MeshRenderer)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_Mesh),
			SERIALIZE(m_Material),
			SERIALIZE(m_Transparent),
			SERIALIZE(m_FindRenderTextureCameraHack)
		);
	}

}
