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
private:
	float m_rotVal;
	float m_rotSpeed;
};

