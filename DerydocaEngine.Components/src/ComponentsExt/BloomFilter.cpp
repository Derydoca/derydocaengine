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
		m_PostProcessCamera = getComponentInChildren<Components::Camera>();
		if (m_PostProcessCamera == nullptr)
		{
			LOG_ERROR("No camera was found attached to this EdgeDetectionFilter component. A camera with a render texture is required to use this component.");
			return;
		}

		float sum = 0.0f;
		float sigma2 = 4.0f;
		m_Weights.resize(10);
		m_Weights[0] = gauss(0, sigma2);
		sum = m_Weights[0];
		for (int i = 1; i < 10; i++)
		{
			m_Weights[i] = gauss(float(i), sigma2);
			sum += 2 * m_Weights[i];
		}

		for (int i = 0; i < 10; i++)
		{
			m_Weights[i] = m_Weights[i] / sum;
		}

		// Create the blur texture
		auto cameraRenderTexture = m_PostProcessCamera->getRenderTexture();
		int width = cameraRenderTexture->getWidth();
		int height = cameraRenderTexture->getHeight();
		m_BlurTex = std::make_shared<Rendering::RenderTexture>();
		m_BlurTex->initializeTexture(width, height);
		m_BlurTex2 = std::make_shared<Rendering::RenderTexture>();
		m_BlurTex2->initializeTexture(width, height);

		//auto shader = m_PostProcessCamera->getPostProcessShader();
		ResourceRef<Resources::ShaderResource> shaderResource = ObjectLibrary::getInstance().getResource<Resources::ShaderResource>("dce99f15-ae1a-44b3-be03-76276da752d1");

		if (shaderResource)
		{
			Rendering::RenderPass subPassNames[3] =
			{
				Rendering::RenderPass("lumThreshPass", m_BlurTex, "BlurTex"),
				Rendering::RenderPass("blurY", m_BlurTex2, "BlurTex2"),
				Rendering::RenderPass("blurX")
			};
			shaderResource.As<Rendering::Shader>()->setSubPasses(GL_FRAGMENT_SHADER, subPassNames, 3);
		}

		updateShader();

		m_PostProcessCamera->setPostProcessShader(shaderResource);
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
		auto material = m_PostProcessCamera->getPostProcessMaterial();
		if (material == nullptr)
		{
			return;
		}

		material->setFloatArray("Weights", m_Weights);
		material->setFloat("LumThresh", m_Threshold);
	}

	SERIALIZE_FUNC_LOAD(archive, BloomFilter)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_Threshold)
		);
	}

	SERIALIZE_FUNC_SAVE(archive, BloomFilter)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_Threshold)
		);
	}

}
