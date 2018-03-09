#pragma once
#include "GameComponent.h"

class Rotator : public GameComponent
{
public:
	GENINSTANCE(Rotator);
	Rotator();
	Rotator(float rotationSpeed);
	~Rotator();

	void update(float deltaTime);

	void deserialize(YAML::Node node);

	inline void setSpeed(float rotSpeed) { m_rotSpeed = rotSpeed; }
	inline float getSpeed() const { return m_rotSpeed; }
private:
	float m_rotVal;
	float m_rotSpeed;
};

