#pragma once
#include <glm\glm.hpp>
#include "Transform.h"
#include "Keyboard.h"
#include "Mouse.h"

class WasdMover
{
public:
	WasdMover(Transform* transform, Keyboard* keyboard, Mouse* mouse) :
		m_transform(transform),
		m_keyboard(keyboard),
		m_mouse(mouse),
		m_moveSpeed(10.0f),
		m_fastMoveMultiplier(2.0f),
		m_slowMoveMultiplier(0.5f),
		m_minXRot(-1.0f),
		m_maxXRot(1.0f)
	{
	}
	~WasdMover();

	void update(float deltaTime);
private:
	Transform* m_transform;
	Keyboard* m_keyboard;
	Mouse* m_mouse;
	float m_moveSpeed;
	float m_fastMoveMultiplier;
	float m_slowMoveMultiplier;
	glm::vec3 m_localMomentum;
	glm::vec2 m_eulerRot;
	float m_minXRot, m_maxXRot;
};

