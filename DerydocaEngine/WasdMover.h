#pragma once
#include <glm\glm.hpp>
#include "Transform.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Key.h"
#include "GameComponent.h"
#include "GameObject.h"

class WasdMover : public GameComponent
{
public:
	WasdMover(Keyboard* keyboard, Mouse* mouse) :
		m_keyboard(keyboard),
		m_mouse(mouse),
		m_moveSpeed(5.0f),
		m_mouseSensitivityX(0.005f),
		m_mouseSensitivityY(0.005f),
		m_fastMoveMultiplier(2.0f),
		m_slowMoveMultiplier(0.5f),
		m_minXRot(-glm::half_pi<float>()),
		m_maxXRot(glm::half_pi<float>())
	{
	}
	~WasdMover();
	
	void init();
	void update(float deltaTime);
private:
	Transform* m_transform;
	Keyboard* m_keyboard;
	Mouse* m_mouse;
	float m_moveSpeed;
	float m_mouseSensitivityX, m_mouseSensitivityY;
	float m_fastMoveMultiplier;
	float m_slowMoveMultiplier;
	glm::vec3 m_localMomentum;
	glm::vec2 m_eulerRot;
	float m_minXRot, m_maxXRot;
};

