#include "EdgeDetectionFilter.h"
#include "Camera.h"
#include "RenderTexture.h"
#include "MeshRenderer.h"
#include "Shader.h"

EdgeDetectionFilter::EdgeDetectionFilter()
{
}


EdgeDetectionFilter::~EdgeDetectionFilter()
{
}

void EdgeDetectionFilter::init()
{
	m_postProcessCamera = getComponent<Camera>();
	if (m_postProcessCamera == nullptr)
	{
		std::cout << "No camera was found attached to this EdgeDetectionFilter component. A camera with a render texture is required to use this component.\n";
		return;
	}

	updateShader();
}

void EdgeDetectionFilter::deserialize(YAML::Node const& compNode)
{
	YAML::Node edgeThresholdNode = compNode["edgeThreshold"];
	if (edgeThresholdNode)
	{
		m_edgeThreshold = edgeThresholdNode.as<float>();
	}

}

void EdgeDetectionFilter::update(float const& deltaTime)
{
	updateShader();
}

void EdgeDetectionFilter::updateShader()
{
	Shader* shader = m_postProcessCamera->getPostProcessShader();
	if (shader == nullptr)
	{
		return;
	}

	shader->setFloat("EdgeThreshold", m_edgeThreshold);
}
