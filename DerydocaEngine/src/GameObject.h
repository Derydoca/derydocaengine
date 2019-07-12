#pragma once
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <vector>
#include "Components\Transform.h"
#include "Object.h"

namespace DerydocaEngine {
	namespace Components {
		class GameComponent;
		struct Transform;
	}
	namespace Rendering {
		struct Projection;
		class Material;
		class MatrixStack;
	}
}

namespace DerydocaEngine
{
	class GameObject: public std::enable_shared_from_this<GameObject>, public Object
	{
	public:
		REGISTER_TYPE_ID(GameObject);
		GameObject(const std::string& name);
		GameObject(const boost::uuids::uuid id, const std::string& name);
		~GameObject();

		void addChild(const std::shared_ptr<GameObject> gameObject);
		void addComponent(const std::shared_ptr<Components::GameComponent> component);
		template <typename T>
		void addComponent(const std::shared_ptr<T> component)
		{
			auto cmp = std::static_pointer_cast<Components::GameComponent>(component);
			addComponent(cmp);
		}
		void renderMesh(
			const std::shared_ptr<Rendering::MatrixStack> matrixStack,
			std::shared_ptr<Rendering::Material> material,
			const Rendering::Projection& projection,
			std::shared_ptr<Components::Transform> projectionTransform
		) const;

		void init();
		void postInit();
		void postRender();
		void preDestroy();
		void preRender();
		void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack) const;
		void renderEditorGUI();
		void update(const float deltaTime);

		void destroy();
		std::vector<std::shared_ptr<GameObject>> getChildren() const { return m_children; }
		std::vector<std::shared_ptr<Components::GameComponent>> getComponents() const { return m_components; }
		std::string getName() const { return m_name; }
		std::string& getName() { return m_name; }
		bool hasParent() const { return !m_parent.expired(); }
		std::shared_ptr<GameObject> getParent() const { return m_parent.lock(); }
		std::shared_ptr<Components::Transform> getTransform() const { return m_transform; }
		void setName(const std::string& name) { m_name = name; }
		void destroyFlaggedChildren();

		template <class T>
		std::shared_ptr<T> getComponentInChildren()
		{
			for (std::vector<std::shared_ptr<Components::GameComponent>>::iterator it = m_components.begin(); it != m_components.end(); ++it)
			{
				// Attempt to cast this object to the type we want
				std::shared_ptr<Components::GameComponent> component = *it;
				auto attemptedCast = std::dynamic_pointer_cast<T>(component);

				// If the cast worked, return the component
				if (attemptedCast != nullptr)
				{
					return attemptedCast;
				}
			}

			for (std::vector<std::shared_ptr<GameObject>>::iterator it = m_children.begin(); it != m_children.end(); ++it)
			{
				return (*it)->getComponentInChildren<T>();
			}

			return nullptr;
		}

		template <class T>
		std::shared_ptr<T> getComponentInChildren(boost::uuids::uuid id)
		{
			for (std::vector<std::shared_ptr<Components::GameComponent>>::iterator it = m_components.begin(); it != m_components.end(); ++it)
			{
				// Attempt to cast this object to the type we want
				std::shared_ptr<Components::GameComponent> component = *it;
				auto attemptedCast = std::dynamic_pointer_cast<T>(component);

				// If the cast worked, return the component
				if (attemptedCast != nullptr && component->getId() == id)
				{
					return attemptedCast;
				}
			}

			for (std::vector<std::shared_ptr<GameObject>>::iterator it = m_children.begin(); it != m_children.end(); ++it)
			{
				return (*it)->getComponentInChildren<T>(id);
			}

			return nullptr;
		}

	private:
		std::string m_name;
		std::shared_ptr<Components::Transform> m_transform;
		std::weak_ptr<GameObject> m_parent;
		std::vector<std::shared_ptr<GameObject>> m_children;
		std::vector<std::shared_ptr<Components::GameComponent>> m_components;
		bool m_destroyFlag;
	};

}