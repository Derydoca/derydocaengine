#pragma once
#include "Input\Keyboard.h"
#include "Components\GameComponent.h"
#include "Components\Transform.h"

namespace DerydocaEngine::Components
{

	class KeyboardMover : public GameComponent, SelfRegister<KeyboardMover>
	{
	public:
		GENINSTANCE(KeyboardMover);
		SERIALIZE_FUNC_DEFINITIONS;
		KeyboardMover();
		KeyboardMover(
			Input::Keyboard* const& keyboard,
			float const& movementSpeed,
			int const& keyForward,
			int const& keyBackward,
			int const& keyLeft,
			int const& keyRight,
			int const& keyUp,
			int const& keyDown);
		~KeyboardMover();

		void init();
		void update(const float deltaTime);

	private:
		float m_MovementSpeed;
		int m_KeyForward;
		int m_KeyBackward;
		int m_KeyLeft;
		int m_KeyRight;
		int m_KeyUp;
		int m_KeyDown;

		std::shared_ptr<Transform> m_Transform;
		Input::Keyboard* m_Keyboard;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::KeyboardMover, 0);
