#include "EngineComponentsPch.h"
#include "Rotator.h"
#include "GameObject.h"

namespace DerydocaEngine::Components
{

	Rotator::Rotator() :
		m_rotVal(0.0f),
		m_rotSpeed(1.0f)
	{
	}

	Rotator::Rotator(float rotationSpeed) :
		m_rotVal(0.0f),
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

}
