#include "GameObject.h"

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::render(MatrixStack* matrixStack) {
	matrixStack->push(m_transform.getModel());

	for each (GameComponent* c in m_components)
	{
		c->render(matrixStack);
	}

	for each (GameObject* go in m_children)
	{
		go->render(matrixStack);
	}

	matrixStack->pop();
}

void GameObject::init()
{
	for each (GameComponent* c in m_components)
	{
		c->init();
	}

	for each (GameObject* go in m_children)
	{
		go->init();
	}
}

void GameObject::update(float deltaTime) {
	for each (GameComponent* c in m_components)
	{
		c->update(deltaTime);
	}

	for each (GameObject* go in m_children)
	{
		go->update(deltaTime);
	}
}

void GameObject::postRender() {
	for each (GameComponent* c in m_components)
	{
		c->postRender();
	}

	for each (GameObject* go in m_children)
	{
		go->postRender();
	}
}

void GameObject::addChild(GameObject* gameObject)
{
	m_children.push_back(gameObject);
}

void GameObject::addComponent(GameComponent* component)
{
	m_components.push_back(component);
	component->setGameObject(this);
}
