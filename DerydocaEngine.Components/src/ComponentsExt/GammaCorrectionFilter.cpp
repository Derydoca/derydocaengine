#include "EngineComponentsPch.h"
#include "GammaCorrectionFilter.h"
#include "Rendering\Shader.h"
#include <iostream>
#include "Rendering\Material.h"

namespace DerydocaEngine::Ext
{

	GammaCorrectionFilter::GammaCorrectionFilter() :
		m_Gamma(2.0f),
		m_PostProcessCamera()
	{
	}

	GammaCorrectionFilter::~GammaCorrectionFilter()
	{
	}

	void GammaCorrectionFilter::init()
	{
		m_PostProcessCamera = getComponentInChildren<Components::Camera>();
		if (m_PostProcessCamera == nullptr)
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
			m_Gamma = lumThreshNode.as<float>();
		}
	}

	void GammaCorrectionFilter::update(const float deltaTime)
	{
		updateShader();
	}

	void GammaCorrectionFilter::updateShader()
	{
		auto mat = m_PostProcessCamera->getPostProcessMaterial();
		if (mat == nullptr)
		{
			return;
		}

		mat->setFloat("Gamma", m_Gamma);
	}

}