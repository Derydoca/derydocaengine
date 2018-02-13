#pragma once
#include "Keyboard.h"
#include "GameComponent.h"
#include "Transform.h"

class KeyboardMover : public GameComponent
{
public:
	GENINSTANCE(KeyboardMover);
	KeyboardMover();
	KeyboardMover(
		Keyboard* keyboard,
		float movementSpeed,
		int keyForward,
		int keyBackward,
		int keyLeft,
		int keyRight,
		int keyUp,
		int keyDown);
	~KeyboardMover();

	void init();
	void update(float deltaTime);

	bool deserialize(YAML::Node node);
private:
	Transform* m_transform;
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

