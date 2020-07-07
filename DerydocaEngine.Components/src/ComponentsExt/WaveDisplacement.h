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

		float& getK() { return m_K; }
		float& getVelocity() { return m_Velocity; }
		float& getAmplitude() { return m_Amplitude; }

		SERIALIZE_FUNC_BASE(
			DerydocaEngine::Components::GameComponent,
			SERIALIZE(m_K),
			SERIALIZE(m_Velocity),
			SERIALIZE(m_Amplitude)
		);

	private:
		float m_K;
		float m_Velocity;
		float m_Amplitude;

		float m_Time;
		std::shared_ptr<Rendering::Material> m_Material;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::WaveDisplacement);
