#include "GameObject.h"

GameObject::GameObject()
{
	m_transform.setGameObject(this);
}

GameObject::GameObject(std::string name)
{
	m_transform.setGameObject(this);
	setName(name);
}


GameObject::~GameObject()
{
	// Delete all of the components attached to this object
	for (auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		delete(*it);
	}
	m_components.clear();

	// Delete all of the children of this object
	for (auto it = m_children.begin(); it != m_children.end(); ++it)
	{
		delete(*it);
	}
	m_children.clear();
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
	gameObject->m_parent = this;
}

void GameObject::addComponent(GameComponent* component)
{
	m_components.push_back(component);
	component->setGameObject(this);
}