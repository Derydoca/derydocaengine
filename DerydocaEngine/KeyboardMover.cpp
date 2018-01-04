#include "KeyboardMover.h"
#include "GameObject.h"
#include "glm\glm.hpp"

KeyboardMover::KeyboardMover(Keyboard * keyboard, float movementSpeed, int keyForward, int keyBackward, int keyLeft, int keyRight, int keyUp, int keyDown) :
	m_keyboard(keyboard),
	m_movementSpeed(movementSpeed),
	m_keyForward(keyForward),
	m_keyBackward(keyBackward),
	m_keyLeft(keyLeft),
	m_keyRight(keyRight),
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

void KeyboardMover::update(float deltaTime)
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
