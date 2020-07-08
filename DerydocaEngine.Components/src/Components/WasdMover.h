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
		~WasdMover();

		void init();
		void update(const float deltaTime);

		void deserialize(const YAML::Node& node);

		SERIALIZE_FUNC_BASE(DerydocaEngine::Components::GameComponent,
			SERIALIZE(m_MoveSpeed),
			SERIALIZE(m_MouseSensitivityX),
			SERIALIZE(m_MouseSensitivityY),
			SERIALIZE(m_FastMoveMultiplier),
			SERIALIZE(m_SlowMoveMultiplier),
			SERIALIZE(m_MinXRot),
			SERIALIZE(m_MaxXRot)
		);
	private:
		float m_MoveSpeed;
		float m_MouseSensitivityX, m_MouseSensitivityY;
		float m_FastMoveMultiplier;
		float m_SlowMoveMultiplier;
		float m_MinXRot, m_MaxXRot;

		std::shared_ptr<Components::Transform> m_Transform;
		Input::Keyboard* m_Keyboard;
		Input::Mouse* m_Mouse;
		glm::vec3 m_LocalMomentum;
		glm::vec2 m_EulerRot;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::WasdMover, 0);
