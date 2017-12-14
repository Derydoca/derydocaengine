#pragma once

class GameObject;
class MatrixStack;
class Camera;

class GameComponent {
public:
	virtual void init() {}
	virtual void update(float deltaTime) {}
	virtual void render(Camera* camera, MatrixStack* matrixStack) {}
	virtual void postRender() {}
	inline void setGameObject(GameObject* gameObject) { m_gameObject = gameObject; }
	inline GameObject* getGameObject() { return m_gameObject; }
private:
	GameObject* m_gameObject;
};