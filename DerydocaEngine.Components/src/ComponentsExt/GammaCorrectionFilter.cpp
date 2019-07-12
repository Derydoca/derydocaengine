#include "EngineComponentsPch.h"
#include "GammaCorrectionFilter.h"
#include "Rendering\Shader.h"
#include <iostream>
#include "Rendering\Material.h"

namespace DerydocaEngine::Ext
{

	GammaCorrectionFilter::GammaCorrectionFilter() :
		m_gamma(2.0f),
		m_postProcessCamera()
	{
	}

	GammaCorrectionFilter::~GammaCorrectionFilter()
	{
	}

	void GammaCorrectionFilter::init()
	{
		m_postProcessCamera = getComponentInChildren<Components::Camera>();
		if (m_postProcessCamera == nullptr)
		{
			LOG_ERROR("No camera was found attached to this EdgeDetectionFilter component. A camera with a render texture is required to use this component.");
			return;
		}
		updateShader();
	}

	void GammaCorrectionFilter::deserialize(const YAML::Node& compNode)
	{
		YAML::Node lumThreshNode = compNode["gamma"];
		if (lumThreshNode)
		{
			m_gamma = lumThreshNode.as<float>();
		}
	}

	void GammaCorrectionFilter::update(const float deltaTime)
	{
		updateShader();
	}

	void GammaCorrectionFilter::updateShader()
	{
		auto mat = m_postProcessCamera->getPostProcessMaterial();
		if (mat == nullptr)
		{
			return;
		}

		mat->setFloat("Gamma", m_gamma);
	}

}