#pragma once
#include <memory>
#include <vector>
#include "Components\Transform.h"

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
	class GameObject: public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject(const std::string& name);

		~GameObject();

		void addChild(const std::shared_ptr<GameObject> gameObject);

		void addComponent(Components::GameComponent* component);

		std::vector<std::shared_ptr<GameObject>> getChildren() const { return m_children; }

		std::vector<Components::GameComponent*> getComponents() const { return m_components; }

		std::shared_ptr<GameObject> getParent() const { return m_parent; }

		std::shared_ptr<Components::Transform> getTransform() { return m_transform; }

		std::string getName() const { return m_name; }

		void init();

		void postInit();

		void postRender();

		void preRender();

		void render(const std::shared_ptr<Rendering::MatrixStack> matrixStack) const;

		void renderMesh(
			const std::shared_ptr<Rendering::MatrixStack> matrixStack,
			Rendering::Material*& material,
			const Rendering::Projection& projection,
			std::shared_ptr<Components::Transform> projectionTransform
		) const;

		void setName(const std::string& name) { m_name = name; }

		void update(const float& deltaTime);

	private:
		std::string m_name;
		std::shared_ptr<Components::Transform> m_transform;
		std::shared_ptr<GameObject> m_parent;
		std::vector<std::shared_ptr<GameObject>> m_children;
		std::vector<Components::GameComponent*> m_components;
	};

}