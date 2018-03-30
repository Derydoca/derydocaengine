#include "GaussianBlurFilter.h"
#include "Shader.h"

using namespace std;

GaussianBlurFilter::GaussianBlurFilter()
{
}


GaussianBlurFilter::~GaussianBlurFilter()
{
}

void GaussianBlurFilter::init()
{
	m_postProcessCamera = getComponent<Camera>();
	if (m_postProcessCamera == nullptr)
	{
		cout << "No camera was found attached to this EdgeDetectionFilter component. A camera with a render texture is required to use this component." << endl;
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

	Shader* shader = m_postProcessCamera->getPostProcessShader();
	if (shader != nullptr)
	{
		std::string subPassNames[2] = { "pass1", "pass2" };
		shader->setSubPasses(GL_FRAGMENT_SHADER, subPassNames, 2);
	}

	updateShader();
}

void GaussianBlurFilter::deserialize(YAML::Node compNode)
{
}

void GaussianBlurFilter::update(float deltaTime)
{
	updateShader();
}

float GaussianBlurFilter::gauss(float x, float sigma2)
{
	double coeff = 1.0 / (glm::two_pi<double>() * sigma2);
	double expon = -(x*x) / (2.0 * sigma2);
	return (float)(coeff * exp(expon));
}

void GaussianBlurFilter::updateShader()
{
	Shader* shader = m_postProcessCamera->getPostProcessShader();
	if (shader == nullptr)
	{
		return;
	}

	shader->setFloatArray("Weights", m_weights, 5);
}
