#include "Rotator.h"
#include "GameObject.h"

Rotator::Rotator()
{
}

Rotator::Rotator(float rotationSpeed) :
	m_rotSpeed(rotationSpeed)
{
}


Rotator::~Rotator()
{
}

void Rotator::update(float const& deltaTime)
{
	m_rotVal += deltaTime * m_rotSpeed;
	getGameObject()->getTransform()->setEulerAngles(glm::vec3(0, m_rotVal, 0));
}

void Rotator::deserialize(YAML::Node const& node)
{
	m_rotSpeed = node["rotationSpeed"].as<float>();
}
