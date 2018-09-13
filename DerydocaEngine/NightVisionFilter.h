#pragma once
#include "GameComponent.h"
#include "Camera.h"

namespace DerydocaEngine::Ext
{

	class NightVisionFilter : public GameComponent
	{
	public:
		GENINSTANCE(NightVisionFilter);

		NightVisionFilter() {}
		~NightVisionFilter() {}

		virtual void init();
		virtual void deserialize(YAML::Node const& compNode);
	private:
		float m_radius = 100.0f;
		DerydocaEngine::Components::Camera* m_postProcessCamera;
	};

}
