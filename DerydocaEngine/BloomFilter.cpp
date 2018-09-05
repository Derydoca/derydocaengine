#include "BloomFilter.h"
#include "Shader.h"

using namespace std;

BloomFilter::BloomFilter()
{
}


BloomFilter::~BloomFilter()
{
	delete m_blurTex;
	delete m_blurTex2;
}

void BloomFilter::init()
{
	m_postProcessCamera = getComponent<Camera>();
	if (m_postProcessCamera == nullptr)
	{
		cout << "No camera was found attached to this EdgeDetectionFilter component. A camera with a render texture is required to use this component." << endl;
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
	RenderTexture* cameraRenderTexture = m_postProcessCamera->getRenderTexture();
	int width = cameraRenderTexture->getWidth();
	int height = cameraRenderTexture->getHeight();
	delete m_blurTex;
	delete m_blurTex2;
	m_blurTex = new RenderTexture(width, height);
	m_blurTex2 = new RenderTexture(width, height);

	Shader* shader = m_postProcessCamera->getPostProcessShader();
	if (shader != nullptr)
	{
		RenderPass subPassNames[3] =
			{
				RenderPass("lumThreshPass", m_blurTex, "BlurTex"),
				RenderPass("blurY", m_blurTex2, "BlurTex2"),
				"blurX"
			};
		shader->setSubPasses(GL_FRAGMENT_SHADER, subPassNames, 3);
	}

	updateShader();
}

void BloomFilter::deserialize(YAML::Node const& compNode)
{
	YAML::Node lumThreshNode = compNode["lumThresh"];
	if (lumThreshNode)
	{
		m_lumThresh = compNode["lumThresh"].as<float>();
	}
}

void BloomFilter::update(float const& deltaTime)
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
	Shader* shader = m_postProcessCamera->getPostProcessShader();
	if (shader == nullptr)
	{
		return;
	}

	shader->setFloatArray("Weights", m_weights, 10);
	shader->setFloat("LumThresh", m_lumThresh);
}
