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
		static std::shared_ptr<GameObject> generate(std::string name = "")
		{
			return std::make_shared<GameObject>();
		}
		GameObject();
		GameObject(const std::string& name);
		~GameObject();

		void init();
		void postInit();
		void update(const float& deltaTime);
		void render(const std::shared_ptr<Rendering::MatrixStack>& matrixStack);
		void renderMesh(
			const std::shared_ptr<Rendering::MatrixStack> matrixStack,
			Rendering::Material*& renderMesh,
			const Rendering::Projection& projection,
			const Components::Transform* projectionTransform
		) const;
		void preRender();
		void postRender();

		void addChild(const std::shared_ptr<GameObject> gameObject);
		void addComponent(Components::GameComponent* const& component);

		inline Components::Transform* getTransform() { return &m_transform; }
		inline std::vector<std::shared_ptr<GameObject>> getChildren() const { return m_children; }
		inline std::vector<Components::GameComponent*> getComponents() const { return m_components; }

		inline std::shared_ptr<GameObject> getParent() const { return m_parent; }
		inline std::string getName() const { return m_name; }

		inline void setName(std::string name) { m_name = name; }
	private:
		std::string m_name;
		Components::Transform m_transform;
		std::shared_ptr<GameObject> m_parent;
		std::vector<std::shared_ptr<GameObject>> m_children;
		std::vector<Components::GameComponent*> m_components;
	};

}