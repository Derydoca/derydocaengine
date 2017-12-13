#pragma once
#include "GameComponent.h"

class Rotator : public GameComponent
{
public:
	Rotator(float rotationSpeed);
	~Rotator();

	void render(Camera* camera, MatrixStack* matrixStack);
	void update(float deltaTime);
	void postRender();
private:
	float m_rotVal;
	float m_rotSpeed;
};

