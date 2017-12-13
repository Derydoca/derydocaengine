#pragma once
#include "GameComponent.h"

class Rotator : public GameComponent
{
public:
	Rotator();
	~Rotator();

	void render(Camera* camera, MatrixStack* matrixStack);
	void update(float deltaTime);
private:
	float m_rotVal;
};

