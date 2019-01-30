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

		std::vector<std::shared_ptr<GameObject>> getChildren() const { return m_children; }
		std::vector<std::shared_ptr<Components::GameComponent>> getComponents() const { return m_components; }
		std::string getName() const { return m_name; }
		std::string& getName() { return m_name; }
		std::shared_ptr<GameObject> getParent() const { return m_parent.lock(); }
		std::shared_ptr<Components::Transform> getTransform() const { return m_transform; }
		void setName(const std::string& name) { m_name = name; }

	private:
		std::string m_name;
		std::shared_ptr<Components::Transform> m_transform;
		std::weak_ptr<GameObject> m_parent;
		std::vector<std::shared_ptr<GameObject>> m_children;
		std::vector<std::shared_ptr<Components::GameComponent>> m_components;
	};

}