#pragma once
#include <glm\glm.hpp>
#include "Components\Transform.h"
#include "Components\GameComponent.h"

namespace DerydocaEngine {
	namespace Input {
		class Keyboard;
		class Mouse;
	}
}

namespace DerydocaEngine::Components
{

	class WasdMover : public GameComponent, SelfRegister<WasdMover>
	{
	public:
		GENINSTANCE(WasdMover);
		WasdMover();
		WasdMover(Input::Keyboard* const& keyboard, Input::Mouse* const& mouse) :
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
		void update(float const& deltaTime);

		void deserialize(YAML::Node const& node);
	private:
		std::shared_ptr<Components::Transform> m_transform;
		Input::Keyboard* m_keyboard;
		Input::Mouse* m_mouse;
		float m_moveSpeed;
		float m_mouseSensitivityX, m_mouseSensitivityY;
		float m_fastMoveMultiplier;
		float m_slowMoveMultiplier;
		glm::vec3 m_localMomentum;
		glm::vec2 m_eulerRot;
		float m_minXRot, m_maxXRot;
	};

}
