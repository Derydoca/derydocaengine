#include "EngineComponentsPch.h"
#include "Rotator.h"
#include "GameObject.h"

namespace DerydocaEngine::Components
{

	Rotator::Rotator() :
		m_Value(0.0f),
		m_Speed(1.0f)
	{
	}

	Rotator::Rotator(float rotationSpeed) :
		m_Value(0.0f),
		m_Speed(rotationSpeed)
	{
	}


	Rotator::~Rotator()
	{
	}

	void Rotator::update(const float deltaTime)
	{
		m_Value += deltaTime * m_Speed;
		getGameObject()->getTransform()->setEulerAngles(glm::vec3(0, m_Value, 0));
	}

	void Rotator::deserialize(const YAML::Node& node)
	{
		m_Speed = node["rotationSpeed"].as<float>();
	}

}
