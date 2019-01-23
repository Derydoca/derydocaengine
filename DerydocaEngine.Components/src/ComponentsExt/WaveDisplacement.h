#pragma once
#include "Components\GameComponent.h"

namespace DerydocaEngine::Ext
{

	class WaveDisplacement : public Components::GameComponent, Components::SelfRegister<WaveDisplacement>
	{
	public:
		GENINSTANCE(WaveDisplacement);

		WaveDisplacement();
		~WaveDisplacement();

		virtual void init();
		virtual void update(const float deltaTime);
		virtual void preRender();
		virtual void deserialize(const YAML::Node& compNode);

		float& getK() { return m_k; }
		float& getVelocity() { return m_velocity; }
		float& getAmplitude() { return m_amplitude; }

	private:
		float m_k;
		float m_velocity;
		float m_amplitude;
		float m_time;
		std::shared_ptr<Rendering::Material> m_material;
	};

}
