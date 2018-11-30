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
		m_postProcessCamera = getComponent<Components::Camera>();
		if (m_postProcessCamera == nullptr)
		{
			std::cout << "No camera was found attached to this EdgeDetectionFilter component. A camera with a render texture is required to use this component.\n";
			return;
		}

		float sum = 0.0f;
		float sigma2 = 8.0f;
		m_weights[0] = gauss(0, sigma2);
		sum = m_weights[0];
		for (int i = 1; i < 5; i++)
		{
			m_weights[i] = gauss(float(i), sigma2);
			sum += 2 * m_weights[i];
		}

		for (int i = 0; i < 5; i++)
		{
			m_weights[i] = m_weights[i] / sum;
		}

		auto shader = m_postProcessCamera->getPostProcessShader();
		if (shader != nullptr)
		{
			Rendering::RenderPass subPasses[2] = { Rendering::RenderPass("pass1"), Rendering::RenderPass("pass2") };
			shader->setSubPasses(GL_FRAGMENT_SHADER, subPasses, 2);
		}

		updateShader();
	}

	void GaussianBlurFilter::deserialize(YAML::Node const& compNode)
	{
	}

	void GaussianBlurFilter::update(float const& deltaTime)
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
		auto shader = m_postProcessCamera->getPostProcessShader();
		if (shader == nullptr)
		{
			return;
		}

		shader->setFloatArray("Weights", m_weights, 5);
	}

}
