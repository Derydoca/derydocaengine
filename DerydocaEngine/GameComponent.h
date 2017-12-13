#pragma once


#include "Camera.h"
//#include "GameObject.h"

class GameObject;
class MatrixStack;

class GameComponent {
public:
	virtual void update(float deltaTime) = 0;
	virtual void render(Camera* camera, MatrixStack* matrixStack) = 0;
	inline void setGameObject(GameObject* gameObject) { m_gameObject = gameObject; }
	inline GameObject* getGameObject() { return m_gameObject; }
private:
	GameObject* m_gameObject;
};