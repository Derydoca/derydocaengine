#pragma once
#include "Components\GameComponent.h"
#include "Components\Camera.h"

namespace DerydocaEngine::Ext
{

	class NightVisionFilter : public Components::GameComponent, Components::SelfRegister<NightVisionFilter>
	{
	public:
		GENINSTANCE(NightVisionFilter);
		SERIALIZE_FUNC_DEFINITIONS;

		NightVisionFilter() {}
		~NightVisionFilter() {}

		virtual void init();
		virtual void deserialize(const YAML::Node& compNode);
		virtual void update(const float deltaTime);

		void updateMaterial();

		float& getRadius() { return m_Radius; }

	private:
		float m_Radius = 100.0f;

		std::shared_ptr<Components::Camera> m_PostProcessCamera;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::NightVisionFilter, 0);
