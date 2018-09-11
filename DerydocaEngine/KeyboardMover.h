#pragma once
#include "Keyboard.h"
#include "GameComponent.h"
#include "Transform.h"

namespace DerydocaEngine::Components
{

	class KeyboardMover : public GameComponent
	{
	public:
		GENINSTANCE(KeyboardMover);
		KeyboardMover();
		KeyboardMover(
			Keyboard* const& keyboard,
			float const& movementSpeed,
			int const& keyForward,
			int const& keyBackward,
			int const& keyLeft,
			int const& keyRight,
			int const& keyUp,
			int const& keyDown);
		~KeyboardMover();

		void init();
		void update(float const& deltaTime);

		void deserialize(YAML::Node const& node);
	private:
		Transform * m_transform;
		Keyboard* m_keyboard;
		float m_movementSpeed;
		int m_keyForward;
		int m_keyBackward;
		int m_keyLeft;
		int m_keyRight;
		int m_key;
		int m_keyUp;
		int m_keyDown;
	};

}
