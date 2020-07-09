#include "EngineComponentsPch.h"
#include "Components\WasdMover.h"

#include "Input\InputManager.h"
#include "Input\Keyboard.h"
#include "Input\Mouse.h"
#include "sdl2\SDL.h"

namespace DerydocaEngine::Components
{

	WasdMover::WasdMover() :
		m_Keyboard(nullptr),
		m_Mouse(nullptr),
		m_MoveSpeed(5.0f),
		m_MouseSensitivityX(0.005f),
		m_MouseSensitivityY(0.005f),
		m_FastMoveMultiplier(2.0f),
		m_SlowMoveMultiplier(0.5f),
		m_LocalMomentum(),
		m_EulerRot(),
		m_MinXRot(-glm::half_pi<float>()),
		m_MaxXRot(glm::half_pi<float>())
	{
		m_Mouse = Input::InputManager::getInstance().getMouse();
		m_Keyboard = Input::InputManager::getInstance().getKeyboard();
	}

	WasdMover::~WasdMover()
	{
	}

	void WasdMover::init()
	{
		m_Transform = getGameObject()->getTransform();
	}

	void WasdMover::update(const float deltaTime) {

		// Set the mouse to relative mode when the right mouse button is down
		if (m_Mouse->isKeyDownFrame(2)) {
			m_Mouse->setRelative(true);
		}
		else if (m_Mouse->isKeyUpFrame(2)) {
			m_Mouse->setRelative(false);
		}

		// Only allow the camera to be moved when the right mouse button is down
		if (m_Mouse->isKeyDown(2)) {
			// Reset the momentum each frame
			m_LocalMomentum = glm::vec3();

			float moveSpeed = m_MoveSpeed * deltaTime;
			if (m_Keyboard->isKeyDown(SDLK_z)) {
				moveSpeed *= m_FastMoveMultiplier;
			}

			// Forward/backward
			if (m_Keyboard->isKeyDown(SDLK_w)) {
				m_LocalMomentum.z -= moveSpeed;
			}
			if (m_Keyboard->isKeyDown(SDLK_s)) {
				m_LocalMomentum.z += moveSpeed;
			}

			// Left/right
			if (m_Keyboard->isKeyDown(SDLK_a)) {
				m_LocalMomentum.x -= moveSpeed;
			}
			if (m_Keyboard->isKeyDown(SDLK_d)) {
				m_LocalMomentum.x += moveSpeed;
			}

			// Up/down
			if (m_Keyboard->isKeyDown(SDLK_q)) {
				m_LocalMomentum.y -= moveSpeed;
			}
			if (m_Keyboard->isKeyDown(SDLK_e)) {
				m_LocalMomentum.y += moveSpeed;
			}
			// Convert the local direction to global direction
			glm::vec4 globalMomentum = m_Transform->getModel() * glm::vec4(m_LocalMomentum, 0);

			// Translate
			m_Transform->setPosition(m_Transform->getPosition() + glm::vec3(globalMomentum));

			// Update the rotations based on mouse movement
			glm::ivec2 diff = m_Mouse->getRelativeMovement();
			m_EulerRot.x -= (float)diff.y * m_MouseSensitivityX;
			m_EulerRot.y -= (float)diff.x * m_MouseSensitivityY;

			// Clamp the vertical look
			if (m_EulerRot.x < m_MinXRot)
			{
				m_EulerRot.x = m_MinXRot;
			}
			else if (m_EulerRot.x > m_MaxXRot)
			{
				m_EulerRot.x = m_MaxXRot;
			}

			if (m_Keyboard->isKeyDownFrame(SDLK_LEFTBRACKET))
			{
				m_EulerRot.y -= 90.0f * 0.0174533f;
			}
			if (m_Keyboard->isKeyDownFrame(SDLK_RIGHTBRACKET))
			{
				m_EulerRot.y += 90.0f * 0.0174533f;
			}

			// Rotate
			glm::fquat newQuat =
				glm::rotate(m_EulerRot.y, glm::vec3(0, 1, 0))
				*
				glm::rotate(m_EulerRot.x, glm::vec3(1, 0, 0))
				;
			m_Transform->setQuaternion(newQuat);
		}
	}

	void WasdMover::deserialize(const YAML::Node& node)
	{
		m_MoveSpeed = node["moveSpeed"].as<float>();
		m_MouseSensitivityX = node["mouseSensitivityX"].as<float>();
		m_MouseSensitivityY = node["mouseSensitivityY"].as<float>();
		m_FastMoveMultiplier = node["fastMoveMultiplier"].as<float>();
		m_SlowMoveMultiplier = node["slowMoveMultiplier"].as<float>();
		m_MinXRot = node["minXRot"].as<float>();
		m_MaxXRot = node["maxXRot"].as<float>();
	}

	SERIALIZE_FUNC_LOAD(archive, WasdMover)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_MoveSpeed),
			SERIALIZE(m_MouseSensitivityX),
			SERIALIZE(m_MouseSensitivityY),
			SERIALIZE(m_FastMoveMultiplier),
			SERIALIZE(m_SlowMoveMultiplier),
			SERIALIZE(m_MinXRot),
			SERIALIZE(m_MaxXRot)
		);
	}

	SERIALIZE_FUNC_SAVE(archive, WasdMover)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_MoveSpeed),
			SERIALIZE(m_MouseSensitivityX),
			SERIALIZE(m_MouseSensitivityY),
			SERIALIZE(m_FastMoveMultiplier),
			SERIALIZE(m_SlowMoveMultiplier),
			SERIALIZE(m_MinXRot),
			SERIALIZE(m_MaxXRot)
		);
	}

}