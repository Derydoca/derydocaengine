#pragma once
#include "Components\GameComponent.h"

namespace DerydocaEngine::Ext
{

	class WaveDisplacement : public Components::GameComponent, Components::SelfRegister<WaveDisplacement>
	{
	public:
		GENINSTANCE(WaveDisplacement);
		SERIALIZE_FUNC_DEFINITIONS;

		WaveDisplacement();
		~WaveDisplacement();

		virtual void init();
		virtual void update(const float deltaTime);
		virtual void preRender();

		float& getK() { return m_K; }
		float& getVelocity() { return m_Velocity; }
		float& getAmplitude() { return m_Amplitude; }

	private:
		float m_K;
		float m_Velocity;
		float m_Amplitude;

		float m_Time;
		std::shared_ptr<Rendering::Material> m_Material;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::WaveDisplacement, 0);
