#include "Rotator.h"
#include "GameObject.h"

Rotator::Rotator()
{
}


Rotator::~Rotator()
{
}

void Rotator::update(float deltaTime)
{
	m_rotVal += deltaTime * 100;
	getGameObject()->getTransform()->setEulerAngles(glm::vec3(0, m_rotVal, 0));
	printf("dt: %f\n", deltaTime);
	printf("rv: %f\n", m_rotVal);
}

void Rotator::render(Camera* camera, MatrixStack* matrixStack)
{

}