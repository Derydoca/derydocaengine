#include "EnginePch.h"
#include "GameObject.h"
#include "Components\GameComponent.h"
#include "Rendering\MatrixStack.h"
#include "cereal\archives\json.hpp"

namespace DerydocaEngine
{
    GameObject::GameObject() :
		Object(),
		m_Name(),
		m_Transform(std::make_shared<Components::Transform>()),
		m_Parent(),
		m_Children(),
		m_Components(),
		m_DestroyFlag(false)
    {
    }

    GameObject::GameObject(const std::string& name) :
		Object(),
		m_Name(name),
		m_Transform(std::make_shared<Components::Transform>()),
		m_Parent(),
		m_Children(),
		m_Components(),
		m_DestroyFlag(false)
	{
		auto idGen = boost::uuids::random_generator_pure();
		m_ID = idGen();
		m_Transform->setGameObject(this);
	}

	GameObject::GameObject(const boost::uuids::uuid id, const std::string & name) :
		Object(id),
		m_Name(name),
		m_Transform(std::make_shared<Components::Transform>()),
		m_Parent(std::weak_ptr<GameObject>()),
		m_Children(),
		m_Components(),
		m_DestroyFlag(false)
	{
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::addChild(const std::shared_ptr<GameObject> gameObject)
	{
		m_Children.push_back(gameObject);
		gameObject->m_Parent = shared_from_this();
	}

	void GameObject::addComponent(const std::shared_ptr<Components::GameComponent> component)
	{
		m_Components.push_back(component);
		component->setGameObject(shared_from_this());
	}

	void GameObject::renderMesh(
		const std::shared_ptr<Rendering::MatrixStack> matrixStack,
		std::shared_ptr<Rendering::Material> material,
		const Rendering::Projection& projection,
		std::shared_ptr<Components::Transform> projectionTransform
	) const
	{
		matrixStack->push(m_Transform->getModel());

		for each (auto c in m_Components)
		{
			c->renderMesh(matrixStack, material, projection, projectionTransform);
		}

		for each (auto go in m_Children)
		{
			go->renderMesh(matrixStack, material, projection, projectionTransform);
		}

		matrixStack->pop();
	}

	void GameObject::init()
	{
		// Use standard for loop so components can be added during init calls
		for (size_t i = 0; i < m_Components.size(); i++)
		{
			m_Components[i]->setGameObject(shared_from_this());
		}

		for (size_t i = 0; i < m_Components.size(); i++)
		{
			m_Components[i]->init();
		}

		for each (auto go in m_Children)
		{
			go->init();
		}
	}

	void GameObject::postInit()
	{
		for each (auto c in m_Components)
		{
			c->postInit();
		}

		for each (auto go in m_Children)
		{
			go->postInit();
		}
	}

	void GameObject::postRender() {
		for each (auto c in m_Components)
		{
			c->postRender();
		}

		for each (auto go in m_Children)
		{
			go->postRender();
		}
	}

	void GameObject::preDestroy()
	{
		for each (auto c in m_Components)
		{
			c->preDestroy();
		}

		for each (auto go in m_Children)
		{
			go->preDestroy();
		}

		m_Components.clear();
		m_Children.clear();
	}

	void GameObject::preRender() {
		for each (auto c in m_Components)
		{
			c->preRender();
		}

		for each (auto go in m_Children)
		{
			go->preRender();
		}
	}

	void GameObject::render(const std::shared_ptr<Rendering::MatrixStack> matrixStack) const {
		matrixStack->push(m_Transform->getModel());

		for each (auto c in m_Components)
		{
			c->render(matrixStack);
		}

		for each (auto go in m_Children)
		{
			go->render(matrixStack);
		}

		matrixStack->pop();
	}

	void GameObject::renderEditorGUI() {
		for each (auto c in m_Components)
		{
			c->renderEditorGUI();
		}

		for each (auto go in m_Children)
		{
			go->renderEditorGUI();
		}
	}

	void GameObject::update(const float deltaTime) {
		for each (auto c in m_Components)
		{
			c->update(deltaTime);
		}

		for each (auto go in m_Children)
		{
			go->update(deltaTime);
		}
	}

	void GameObject::destroy()
	{
		m_Children.clear();
		m_Parent.reset();
		m_DestroyFlag = true;
	}

	void GameObject::destroyFlaggedChildren()
	{
		for (size_t i = m_Children.size(); i > 0 ; i--)
		{
			auto child = m_Children[i-1];
			child->destroyFlaggedChildren();
			if (child->m_DestroyFlag)
			{
				child->preDestroy();
				m_Children.erase(m_Children.begin() + (i - 1));
				D_LOG_TRACE("Deleting '{}' which has {} reference(s).", child->getName().c_str(), child.use_count());
			}
		}
	}

	SERIALIZE_FUNC_LOAD(archive, GameObject)
	{
		archive(
			SERIALIZE(m_Name),
			SERIALIZE(m_Transform),
			SERIALIZE(m_Children),
			SERIALIZE(m_Components)
		);
		m_DestroyFlag = false;
	}

	SERIALIZE_FUNC_SAVE(archive, GameObject)
	{
		archive(
			SERIALIZE(m_Name),
			SERIALIZE(m_Transform),
			SERIALIZE(m_Children),
			SERIALIZE(m_Components)
		);
	}

	template void GameObject::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const version);
	template void GameObject::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const version) const;

}
