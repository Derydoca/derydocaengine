#pragma once
#include <vector>
#include "Transform.h"

struct Projection;
struct Transform;
class GameComponent;
class MatrixStack;
namespace DerydocaEngine::Rendering {
	class Material;
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
	void render(MatrixStack* const& matrixStack);
	void renderMesh(MatrixStack* const& matrixStack, DerydocaEngine::Rendering::Material* const& renderMesh, Projection const& projection, Transform* const& projectionTransform);
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

