#pragma once
#include <vector>
#include "Transform.h"

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

	class GameObject
	{
	public:
		GameObject();
		GameObject(std::string const& name);
		~GameObject();

		void init();
		void postInit();
		void update(float const& deltaTime);
		void render(DerydocaEngine::Rendering::MatrixStack* const& matrixStack);
		void renderMesh(DerydocaEngine::Rendering::MatrixStack* const& matrixStack, DerydocaEngine::Rendering::Material* const& renderMesh, DerydocaEngine::Rendering::Projection const& projection, Components::Transform* const& projectionTransform);
		void preRender();
		void postRender();

		void addChild(GameObject* const& gameObject);
		void addComponent(DerydocaEngine::Components::GameComponent* const& component);

		inline Components::Transform* getTransform() { return &m_transform; }
		inline std::vector<GameObject*> getChildren() const { return m_children; }
		inline std::vector<DerydocaEngine::Components::GameComponent*> getComponents() const { return m_components; }

		inline GameObject* getParent() const { return m_parent; }
		inline std::string getName() const { return m_name; }

		inline void setName(std::string name) { m_name = name; }
	private:
		std::string m_name;
		Components::Transform m_transform;
		GameObject* m_parent;
		std::vector<GameObject*> m_children;
		std::vector<DerydocaEngine::Components::GameComponent*> m_components;
	};

}