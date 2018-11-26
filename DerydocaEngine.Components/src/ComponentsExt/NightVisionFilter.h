#pragma once
#include "Components\GameComponent.h"
#include "Components\Camera.h"

namespace DerydocaEngine::Ext
{

	class NightVisionFilter : public Components::GameComponent, Components::SelfRegister<NightVisionFilter>
	{
	public:
		GENINSTANCE(NightVisionFilter);

		NightVisionFilter() {}
		~NightVisionFilter() {}

		virtual void init();
		virtual void deserialize(YAML::Node const& compNode);
	private:
		float m_radius = 100.0f;
		Components::Camera* m_postProcessCamera;
	};

}
