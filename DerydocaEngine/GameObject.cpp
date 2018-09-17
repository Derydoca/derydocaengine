#include "GameObject.h"
#include "GameComponent.h"
#include "MatrixStack.h"

GameObject::GameObject()
{
	m_transform.setGameObject(this);
}

GameObject::GameObject(std::string const& name)
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

void GameObject::render(DerydocaEngine::Rendering::MatrixStack* const& matrixStack) {
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

void GameObject::renderMesh(DerydocaEngine::Rendering::MatrixStack * const& matrixStack, DerydocaEngine::Rendering::Material * const& material, DerydocaEngine::Rendering::Projection const& projection, Transform* const& projectionTransform)
{
	matrixStack->push(m_transform.getModel());

	for each (GameComponent* c in m_components)
	{
		c->renderMesh(matrixStack, material, projection, projectionTransform);
	}

	for each (GameObject* go in m_children)
	{
		go->renderMesh(matrixStack, material, projection, projectionTransform);
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

void GameObject::postInit()
{
	for each (GameComponent* c in m_components)
	{
		c->postInit();
	}

	for each (GameObject* go in m_children)
	{
		go->postInit();
	}
}

void GameObject::update(float const& deltaTime) {
	for each (GameComponent* c in m_components)
	{
		c->update(deltaTime);
	}

	for each (GameObject* go in m_children)
	{
		go->update(deltaTime);
	}
}

void GameObject::preRender() {
	for each (GameComponent* c in m_components)
	{
		c->preRender();
	}

	for each (GameObject* go in m_children)
	{
		go->preRender();
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

void GameObject::addChild(GameObject* const& gameObject)
{
	m_children.push_back(gameObject);
	gameObject->m_parent = this;
}

void GameObject::addComponent(GameComponent* const& component)
{
	m_components.push_back(component);
	component->setGameObject(this);
}
