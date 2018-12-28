#include "EnginePch.h"
#include "GameObject.h"
#include "Components\GameComponent.h"
#include "Rendering\MatrixStack.h"

namespace DerydocaEngine
{

	GameObject::GameObject(const std::string& name) :
		m_name(name),
		m_transform(std::make_shared<Components::Transform>()),
		m_parent(),
		m_children(),
		m_components()
	{
		m_transform->setGameObject(this);
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::addChild(const std::shared_ptr<GameObject> gameObject)
	{
		m_children.push_back(gameObject);
		gameObject->m_parent = shared_from_this();
	}

	void GameObject::addComponent(std::shared_ptr<Components::GameComponent> component)
	{
		m_components.push_back(component);
		component->setGameObject(shared_from_this());
	}

	void GameObject::init()
	{
		for each (std::shared_ptr<Components::GameComponent> c in m_components)
		{
			c->init();
		}

		for each (std::shared_ptr<GameObject> go in m_children)
		{
			go->init();
		}
	}

	void GameObject::preDestroy()
	{
		for each (std::shared_ptr<Components::GameComponent> c in m_components)
		{
			c->preDestroy();
		}

		for each (std::shared_ptr<GameObject> go in m_children)
		{
			go->preDestroy();
		}

		m_components.clear();
		m_children.clear();
	}

	void GameObject::preRender() {
		for each (std::shared_ptr<Components::GameComponent> c in m_components)
		{
			c->preRender();
		}

		for each (std::shared_ptr<GameObject> go in m_children)
		{
			go->preRender();
		}
	}

	void GameObject::postInit()
	{
		for each (std::shared_ptr<Components::GameComponent> c in m_components)
		{
			c->postInit();
		}

		for each (std::shared_ptr<GameObject> go in m_children)
		{
			go->postInit();
		}
	}

	void GameObject::postRender() {
		for each (std::shared_ptr<Components::GameComponent> c in m_components)
		{
			c->postRender();
		}

		for each (std::shared_ptr<GameObject> go in m_children)
		{
			go->postRender();
		}
	}

	void GameObject::render(const std::shared_ptr<Rendering::MatrixStack> matrixStack) const {
		matrixStack->push(m_transform->getModel());

		for each (std::shared_ptr<Components::GameComponent> c in m_components)
		{
			c->render(matrixStack);
		}

		for each (std::shared_ptr<GameObject> go in m_children)
		{
			go->render(matrixStack);
		}

		matrixStack->pop();
	}

	void GameObject::renderMesh(
		const std::shared_ptr<Rendering::MatrixStack> matrixStack,
		std::shared_ptr<Rendering::Material> material,
		const Rendering::Projection& projection,
		std::shared_ptr<Components::Transform> projectionTransform
	) const
	{
		matrixStack->push(m_transform->getModel());

		for each (std::shared_ptr<Components::GameComponent> c in m_components)
		{
			c->renderMesh(matrixStack, material, projection, projectionTransform);
		}

		for each (std::shared_ptr<GameObject> go in m_children)
		{
			go->renderMesh(matrixStack, material, projection, projectionTransform);
		}

		matrixStack->pop();
	}

	void GameObject::update(const float& deltaTime) {
		for each (std::shared_ptr<Components::GameComponent> c in m_components)
		{
			c->update(deltaTime);
		}

		for each (std::shared_ptr<GameObject> go in m_children)
		{
			go->update(deltaTime);
		}
	}

}
