#include "EngineComponentsPch.h"
#include "KeyboardMover.h"
#include "GameObject.h"
#include "glm\glm.hpp"

namespace DerydocaEngine::Components
{

	KeyboardMover::KeyboardMover() :
		m_Transform(),
		m_Keyboard(nullptr),
		m_MovementSpeed(1.0f),
		m_KeyForward(),
		m_KeyBackward(),
		m_KeyLeft(),
		m_KeyRight(),
		m_KeyUp(),
		m_KeyDown()
	{
	}

	KeyboardMover::KeyboardMover(
		Input::Keyboard * const& keyboard,
		float const& movementSpeed,
		int const& keyForward,
		int const& keyBackward,
		int const& keyLeft,
		int const& keyRight,
		int const& keyUp,
		int const& keyDown
	) :
		m_Transform(),
		m_Keyboard(keyboard),
		m_MovementSpeed(movementSpeed),
		m_KeyForward(keyForward),
		m_KeyBackward(keyBackward),
		m_KeyLeft(keyLeft),
		m_KeyRight(keyRight),
		m_KeyUp(keyUp),
		m_KeyDown(keyDown)
	{
	}

	KeyboardMover::~KeyboardMover()
	{
	}

	void KeyboardMover::init()
	{
		m_Transform = getGameObject()->getTransform();
	}

	void KeyboardMover::update(const float deltaTime)
	{
		float frameSpeed = m_MovementSpeed * deltaTime;
		glm::vec3 frameMovement = glm::vec3(0);

		// Z Movement
		if (m_Keyboard->isKeyDown(m_KeyForward))
		{
			frameMovement.z -= frameSpeed;
		}
		if (m_Keyboard->isKeyDown(m_KeyBackward))
		{
			frameMovement.z += frameSpeed;
		}

		// X Movement
		if (m_Keyboard->isKeyDown(m_KeyRight))
		{
			frameMovement.x += frameSpeed;
		}
		if (m_Keyboard->isKeyDown(m_KeyLeft))
		{
			frameMovement.x -= frameSpeed;
		}

		// Y Movement
		if (m_Keyboard->isKeyDown(m_KeyUp))
		{
			frameMovement.y += frameSpeed;
		}
		if (m_Keyboard->isKeyDown(m_KeyDown))
		{
			frameMovement.y -= frameSpeed;
		}

		// Translate the gameobject
		m_Transform->translate(frameMovement);
	}

	void KeyboardMover::deserialize(const YAML::Node& node)
	{
		m_MovementSpeed = node["movementSpeed"].as<float>();
		m_KeyForward = node["keyForward"].as<int>();
		m_KeyBackward = node["keyBackward"].as<int>();
		m_KeyLeft = node["keyLeft"].as<int>();
		m_KeyRight = node["keyRight"].as<int>();
		m_KeyUp = node["keyUp"].as<int>();
		m_KeyDown = node["keyDown"].as<int>();
	}

}
