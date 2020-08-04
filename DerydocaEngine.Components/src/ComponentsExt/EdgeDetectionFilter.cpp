#include "EngineComponentsPch.h"
#include "EdgeDetectionFilter.h"
#include "Components\Camera.h"
#include "Rendering\RenderTexture.h"
#include "Components\MeshRenderer.h"
#include "Rendering\Shader.h"
#include <iostream>
#include "Rendering\Material.h"
#include "Rendering\RenderPass.h"

namespace DerydocaEngine::Ext
{

	EdgeDetectionFilter::EdgeDetectionFilter() :
		m_EdgeThreshold(0.5f),
		m_PostProcessCamera()
	{
	}

	EdgeDetectionFilter::~EdgeDetectionFilter()
	{
	}

	void EdgeDetectionFilter::init()
	{
		ResourceRef<Resources::ShaderResource> shaderResource = ObjectLibrary::getInstance().getResource<Resources::ShaderResource>("c72aa38d-9cc4-44ea-8ec0-028dc5d13e28");
		if (shaderResource)
		{
			Rendering::RenderPass subPassNames[1] =
			{
				Rendering::RenderPass("edgeDetection")
			};
			shaderResource.As<Rendering::Shader>()->setSubPasses(GL_FRAGMENT_SHADER, subPassNames, 1);
		}

		m_PostProcessCamera = getComponentInChildren<Components::Camera>();
		if (m_PostProcessCamera == nullptr)
		{
			LOG_ERROR("No camera was found attached to this EdgeDetectionFilter component. A camera with a render texture is required to use this component.");
			return;
		}

		updateShader();

		m_PostProcessCamera->setPostProcessShader(shaderResource);
	}

	void EdgeDetectionFilter::update(const float deltaTime)
	{
		updateShader();
	}

	void EdgeDetectionFilter::updateShader()
	{
		auto material = m_PostProcessCamera->getPostProcessMaterial();
		if (material == nullptr)
		{
			return;
		}

		material->setFloat("EdgeThreshold", m_EdgeThreshold);
	}

	SERIALIZE_FUNC_LOAD(archive, EdgeDetectionFilter)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_EdgeThreshold)
		);
	}

	SERIALIZE_FUNC_SAVE(archive, EdgeDetectionFilter)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_EdgeThreshold)
		);
	}

}