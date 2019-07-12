#include "EngineComponentsPch.h"
#include "EdgeDetectionFilter.h"
#include "Components\Camera.h"
#include "Rendering\RenderTexture.h"
#include "Components\MeshRenderer.h"
#include "Rendering\Shader.h"
#include <iostream>
#include "Rendering\Material.h"

namespace DerydocaEngine::Ext
{

	EdgeDetectionFilter::EdgeDetectionFilter() :
		m_edgeThreshold(0.5f),
		m_postProcessCamera()
	{
	}

	EdgeDetectionFilter::~EdgeDetectionFilter()
	{
	}

	void EdgeDetectionFilter::init()
	{
		m_postProcessCamera = getComponentInChildren<Components::Camera>();
		if (m_postProcessCamera == nullptr)
		{
			LOG_ERROR("No camera was found attached to this EdgeDetectionFilter component. A camera with a render texture is required to use this component.");
			return;
		}

		updateShader();
	}

	void EdgeDetectionFilter::deserialize(const YAML::Node& compNode)
	{
		YAML::Node edgeThresholdNode = compNode["edgeThreshold"];
		if (edgeThresholdNode)
		{
			m_edgeThreshold = edgeThresholdNode.as<float>();
		}

	}

	void EdgeDetectionFilter::update(const float deltaTime)
	{
		updateShader();
	}

	void EdgeDetectionFilter::updateShader()
	{
		auto material = m_postProcessCamera->getPostProcessMaterial();
		if (material == nullptr)
		{
			return;
		}

		material->setFloat("EdgeThreshold", m_edgeThreshold);
	}

}