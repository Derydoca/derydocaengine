#pragma once
#include "Components\GameComponent.h"

namespace DerydocaEngine::Components
{

	class Rotator : public GameComponent, SelfRegister<Rotator>
	{
	public:
		GENINSTANCE(Rotator);
		SERIALIZE_FUNC_DEFINITIONS;

		Rotator();
		Rotator(float rotationSpeed);
		~Rotator();

		void update(const float deltaTime);

		inline void setSpeed(float const& rotSpeed) { m_Speed = rotSpeed; }
		inline float& getSpeed() { return m_Speed; }
		inline float getSpeed() const { return m_Speed; }
	private:
		float m_Value;
		float m_Speed;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::Rotator, 0);
