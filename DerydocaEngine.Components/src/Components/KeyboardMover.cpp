#include "EngineComponentsPch.h"
#include "KeyboardMover.h"
#include "GameObject.h"
#include "glm\glm.hpp"

namespace DerydocaEngine::Components
{

	KeyboardMover::KeyboardMover() :
		m_transform(),
		m_keyboard(nullptr),
		m_movementSpeed(1.0f),
		m_keyForward(),
		m_keyBackward(),
		m_keyLeft(),
		m_keyRight(),
		m_key(),
		m_keyUp(),
		m_keyDown()
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
		m_transform(),
		m_keyboard(keyboard),
		m_movementSpeed(movementSpeed),
		m_keyForward(keyForward),
		m_keyBackward(keyBackward),
		m_keyLeft(keyLeft),
		m_keyRight(keyRight),
		m_key(),
		m_keyUp(keyUp),
		m_keyDown(keyDown)
	{
	}

	KeyboardMover::~KeyboardMover()
	{
	}

	void KeyboardMover::init()
	{
		m_transform = getGameObject()->getTransform();
	}

	void KeyboardMover::update(const float deltaTime)
	{
		float frameSpeed = m_movementSpeed * deltaTime;
		glm::vec3 frameMovement = glm::vec3(0);

		// Z Movement
		if (m_keyboard->isKeyDown(m_keyForward))
		{
			frameMovement.z -= frameSpeed;
		}
		if (m_keyboard->isKeyDown(m_keyBackward))
		{
			frameMovement.z += frameSpeed;
		}

		// X Movement
		if (m_keyboard->isKeyDown(m_keyRight))
		{
			frameMovement.x += frameSpeed;
		}
		if (m_keyboard->isKeyDown(m_keyLeft))
		{
			frameMovement.x -= frameSpeed;
		}

		// Y Movement
		if (m_keyboard->isKeyDown(m_keyUp))
		{
			frameMovement.y += frameSpeed;
		}
		if (m_keyboard->isKeyDown(m_keyDown))
		{
			frameMovement.y -= frameSpeed;
		}

		// Translate the gameobject
		m_transform->translate(frameMovement);
	}

	void KeyboardMover::deserialize(const YAML::Node& node)
	{
		m_movementSpeed = node["movementSpeed"].as<float>();
		m_keyForward = node["keyForward"].as<int>();
		m_keyBackward = node["keyBackward"].as<int>();
		m_keyLeft = node["keyLeft"].as<int>();
		m_keyRight = node["keyRight"].as<int>();
		m_keyUp = node["keyUp"].as<int>();
		m_keyDown = node["keyDown"].as<int>();
	}

}
