#pragma once
#include "GameComponent.h"

class Rotator : public GameComponent
{
public:
	Rotator(float rotationSpeed);
	~Rotator();

	void update(float deltaTime);
private:
	float m_rotVal;
	float m_rotSpeed;
};

