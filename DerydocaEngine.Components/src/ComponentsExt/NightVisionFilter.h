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
		virtual void deserialize(const YAML::Node& compNode);
		virtual void update(const float deltaTime);

		float& getRadius() { return m_radius; }
	private:
		float m_radius = 100.0f;
		std::shared_ptr<Components::Camera> m_postProcessCamera;
	};

}
