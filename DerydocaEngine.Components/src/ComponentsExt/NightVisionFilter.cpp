#include "EngineComponentsPch.h"
#include "NightVisionFilter.h"
#include <iostream>
#include "Rendering\Material.h"

namespace DerydocaEngine::Ext
{

	void NightVisionFilter::init()
	{
		m_PostProcessCamera = getComponentInChildren<Components::Camera>();
		if (m_PostProcessCamera == nullptr)
		{
			LOG_ERROR("No camera was found attached to this EdgeDetectionFilter component. A camera with a render texture is required to use this component.");
			return;
		}

		updateMaterial();
	}

	void NightVisionFilter::deserialize(const YAML::Node& compNode)
	{
		YAML::Node radiusNode = compNode["radius"];
		if (radiusNode)
		{
			m_Radius = radiusNode.as<float>();
		}
	}

	void NightVisionFilter::update(const float deltaTime)
	{
		updateMaterial();
	}

	void NightVisionFilter::updateMaterial()
	{
		auto material = m_PostProcessCamera->getPostProcessMaterial();
		if (material)
		{
			material->setFloat("Radius", m_Radius);
		}
	}

	SERIALIZE_FUNC_LOAD(archive, NightVisionFilter)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_Radius)
		);
	}

	SERIALIZE_FUNC_SAVE(archive, NightVisionFilter)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_Radius)
		);
	}

}
