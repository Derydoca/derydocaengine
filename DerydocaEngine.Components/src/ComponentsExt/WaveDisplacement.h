#pragma once
#include "Components\GameComponent.h"

namespace DerydocaEngine::Ext
{

	class WaveDisplacement : public Components::GameComponent, Components::SelfRegister<WaveDisplacement>
	{
	public:
		GENINSTANCE(WaveDisplacement);

		WaveDisplacement() {}
		~WaveDisplacement() {}

		virtual void init();
		virtual void update(const float deltaTime);
		virtual void preRender();
		virtual void deserialize(const YAML::Node& compNode);
	private:
		float m_k = 1.0f;
		float m_velocity = 1.0f;
		float m_amplitude = 0.5f;
		float m_time = 0.0f;
		std::shared_ptr<Rendering::Material> m_material;
	};

}
