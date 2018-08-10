#include "NightVisionFilter.h"



NightVisionFilter::NightVisionFilter()
{
}


NightVisionFilter::~NightVisionFilter()
{
}

void NightVisionFilter::init()
{
	m_postProcessCamera = getComponent<Camera>();
	if (m_postProcessCamera == nullptr)
	{
		cout << "No camera was found attached to this EdgeDetectionFilter component. A camera with a render texture is required to use this component." << endl;
		return;
	}

	m_postProcessCamera->getPostProcessMaterial()->setFloat("Radius", m_radius);
}

void NightVisionFilter::deserialize(YAML::Node compNode)
{
	YAML::Node radiusNode = compNode["radius"];
	if (radiusNode)
	{
		m_radius = radiusNode.as<float>();
	}
}