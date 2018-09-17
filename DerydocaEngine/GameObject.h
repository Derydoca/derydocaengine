#pragma once
#include <vector>
#include "Transform.h"

struct Transform;
class GameComponent;
namespace DerydocaEngine::Rendering {
	struct Projection;
	class Material;
	class MatrixStack;
}

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
	void renderMesh(DerydocaEngine::Rendering::MatrixStack* const& matrixStack, DerydocaEngine::Rendering::Material* const& renderMesh, DerydocaEngine::Rendering::Projection const& projection, Transform* const& projectionTransform);
	void preRender();
	void postRender();

	void addChild(GameObject* const& gameObject);
	void addComponent(GameComponent* const& component);

	inline Transform* getTransform() { return &m_transform; }
	inline std::vector<GameObject*> getChildren() const { return m_children; }
	inline std::vector<GameComponent*> getComponents() const { return m_components; }

	inline GameObject* getParent() const { return m_parent; }
	inline std::string getName() const { return m_name; }

	inline void setName(std::string name) { m_name = name; }
private:
	std::string m_name;
	Transform m_transform;
	GameObject* m_parent;
	std::vector<GameObject*> m_children;
	std::vector<GameComponent*> m_components;
};

