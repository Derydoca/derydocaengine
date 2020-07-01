#include "EngineComponentsPch.h"
#include "GaussianBlurFilter.h"

#include <GL/glew.h>
#include <iostream>
#include "Rendering\Material.h"
#include "Rendering\RenderPass.h"
#include "Rendering\Shader.h"

namespace DerydocaEngine::Ext
{

	void GaussianBlurFilter::init()
	{
		m_PostProcessCamera = getComponentInChildren<Components::Camera>();
		if (m_PostProcessCamera == nullptr)
		{
			LOG_ERROR("No camera was found attached to this EdgeDetectionFilter component. A camera with a render texture is required to use this component.");
			return;
		}

		float sum = 0.0f;
		float sigma2 = 8.0f;
		m_Weights.resize(5);
		m_Weights[0] = gauss(0, sigma2);
		sum = m_Weights[0];
		for (int i = 1; i < 5; i++)
		{
			m_Weights[i] = gauss(float(i), sigma2);
			sum += 2 * m_Weights[i];
		}

		for (int i = 0; i < 5; i++)
		{
			m_Weights[i] = m_Weights[i] / sum;
		}

		auto shader = m_PostProcessCamera->getPostProcessShader();
		if (shader != nullptr)
		{
			Rendering::RenderPass subPasses[2] = { Rendering::RenderPass("pass1"), Rendering::RenderPass("pass2") };
			shader->setSubPasses(GL_FRAGMENT_SHADER, subPasses, 2);
		}

		updateShader();
	}

	void GaussianBlurFilter::deserialize(const YAML::Node& compNode)
	{
	}

	void GaussianBlurFilter::update(const float deltaTime)
	{
		updateShader();
	}

	float GaussianBlurFilter::gauss(float const& x, float const& sigma2)
	{
		double coeff = 1.0 / (glm::two_pi<double>() * sigma2);
		double expon = -(x*x) / (2.0 * sigma2);
		return (float)(coeff * exp(expon));
	}

	void GaussianBlurFilter::updateShader()
	{
		auto material = m_PostProcessCamera->getPostProcessMaterial();
		if (material == nullptr)
		{
			return;
		}

		material->setFloatArray("Weights", m_Weights);
	}

}
