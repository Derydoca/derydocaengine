#pragma once
#include "Components\GameComponent.h"

namespace DerydocaEngine::Components
{

	class Rotator : public GameComponent, SelfRegister<Rotator>
	{
	public:
		GENINSTANCE(Rotator);
		Rotator();
		Rotator(float rotationSpeed);
		~Rotator();

		void update(const float deltaTime);

		void deserialize(const YAML::Node& node);

		inline void setSpeed(float const& rotSpeed) { m_Speed = rotSpeed; }
		inline float& getSpeed() { return m_Speed; }
		inline float getSpeed() const { return m_Speed; }

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::GameComponent,
			SERIALIZE(m_Speed));
	private:
		float m_Value;
		float m_Speed;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::Rotator, 0);
