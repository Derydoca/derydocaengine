#pragma once


#include <vector>
#include "Transform.h"
#include "GameComponent.h"
#include "Camera.h"
#include "MatrixStack.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void init();
	void update(float deltaTime);
	void render(MatrixStack* matrixStack);
	void postRender();

	void addChild(GameObject* gameObject);
	void addComponent(GameComponent* component);

	inline Transform* getTransform() { return &m_transform; }
	inline std::vector<GameObject*> getChildren() const { return m_children; }
	inline std::vector<GameComponent*> getComponents() const { return m_components; }
private:
	Transform m_transform;
	std::vector<GameObject*> m_children;
	std::vector<GameComponent*> m_components;
};

