#include "EngineComponentsPch.h"
#include "BloomFilter.h"

#include <GL/glew.h>
#include <iostream>
#include "Rendering\Material.h"
#include "Rendering\RenderPass.h"
#include "Rendering\Shader.h"

namespace DerydocaEngine::Ext
{

	BloomFilter::BloomFilter()
	{
	}

	BloomFilter::~BloomFilter()
	{
	}

	void BloomFilter::init()
	{
		m_postProcessCamera = getComponent<Components::Camera>();
		if (m_postProcessCamera == nullptr)
		{
			std::cout << "No camera was found attached to this EdgeDetectionFilter component. A camera with a render texture is required to use this component.\n";
			return;
		}

		float sum = 0.0f;
		float sigma2 = 4.0f;
		m_weights[0] = gauss(0, sigma2);
		sum = m_weights[0];
		for (int i = 1; i < 10; i++)
		{
			m_weights[i] = gauss(float(i), sigma2);
			sum += 2 * m_weights[i];
		}

		for (int i = 0; i < 10; i++)
		{
			m_weights[i] = m_weights[i] / sum;
		}

		// Create the blur texture
		auto cameraRenderTexture = m_postProcessCamera->getRenderTexture();
		int width = cameraRenderTexture->getWidth();
		int height = cameraRenderTexture->getHeight();
		m_blurTex = std::make_shared<Rendering::RenderTexture>(width, height);
		m_blurTex2 = std::make_shared<Rendering::RenderTexture>(width, height);

		auto shader = m_postProcessCamera->getPostProcessShader();
		if (shader != nullptr)
		{
			Rendering::RenderPass subPassNames[3] =
			{
				Rendering::RenderPass("lumThreshPass", m_blurTex, "BlurTex"),
				Rendering::RenderPass("blurY", m_blurTex2, "BlurTex2"),
				Rendering::RenderPass("blurX")
			};
			shader->setSubPasses(GL_FRAGMENT_SHADER, subPassNames, 3);
		}

		updateShader();
	}

	void BloomFilter::deserialize(const YAML::Node& compNode)
	{
		YAML::Node lumThreshNode = compNode["lumThresh"];
		if (lumThreshNode)
		{
			m_lumThresh = compNode["lumThresh"].as<float>();
		}
	}

	void BloomFilter::update(const float deltaTime)
	{
		updateShader();
	}

	float BloomFilter::gauss(float const& x, float const& sigma2)
	{
		double coeff = 1.0 / (glm::two_pi<double>() * sigma2);
		double expon = -(x*x) / (2.0 * sigma2);
		return (float)(coeff * exp(expon));
	}

	void BloomFilter::updateShader()
	{
		auto shader = m_postProcessCamera->getPostProcessShader();
		if (shader == nullptr)
		{
			return;
		}

		shader->setFloatArray("Weights", m_weights, 10);
		shader->setFloat("LumThresh", m_lumThresh);
	}

}
