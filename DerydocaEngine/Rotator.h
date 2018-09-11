#pragma once
#include "GameComponent.h"

namespace DerydocaEngine::Components
{

	class Rotator : public GameComponent
	{
	public:
		GENINSTANCE(Rotator);
		Rotator();
		Rotator(float rotationSpeed);
		~Rotator();

		void update(float const& deltaTime);

		void deserialize(YAML::Node const& node);

		inline void setSpeed(float const& rotSpeed) { m_rotSpeed = rotSpeed; }
		inline float getSpeed() const { return m_rotSpeed; }
	private:
		float m_rotVal;
		float m_rotSpeed;
	};

}
