#include "EngineComponentsPch.h"
#include "NightVisionFilter.h"
#include <iostream>
#include "Rendering\Material.h"

namespace DerydocaEngine::Ext
{

	void NightVisionFilter::init()
	{
		m_postProcessCamera = getComponent<Components::Camera>();
		if (m_postProcessCamera == nullptr)
		{
			std::cout << "No camera was found attached to this EdgeDetectionFilter component. A camera with a render texture is required to use this component.\n";
			return;
		}

		updateMaterial();
	}

	void NightVisionFilter::deserialize(const YAML::Node& compNode)
	{
		YAML::Node radiusNode = compNode["radius"];
		if (radiusNode)
		{
			m_radius = radiusNode.as<float>();
		}
	}

	void NightVisionFilter::update(const float deltaTime)
	{
		updateMaterial();
	}

	void NightVisionFilter::updateMaterial()
	{
		m_postProcessCamera->getPostProcessMaterial()->setFloat("Radius", m_radius);
	}

}
