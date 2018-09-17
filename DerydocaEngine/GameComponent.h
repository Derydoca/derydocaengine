#pragma once

#include <boost\uuid\uuid.hpp>
#include <iostream>
#include <vector>
#include "ObjectLibrary.h"
#include "YamlTools.h"

struct Resource;
struct Transform;

class GameObject;
namespace DerydocaEngine::Rendering {
	struct Projection;
	class Material;
	class MatrixStack;
}
class YAML::Node;

#define GENINSTANCE(TYPE) \
	static GameComponent* generateInstance() { return new TYPE(); }

class GameComponent {
public:
	GameComponent() {}
	virtual ~GameComponent() {}
	virtual void init() {}
	virtual void postInit() {}
	virtual void update(float const& deltaTime) {}
	virtual void preRender() {}
	virtual void render(DerydocaEngine::Rendering::MatrixStack* const& matrixStack) {}
	virtual void renderMesh(DerydocaEngine::Rendering::MatrixStack* const& matrixStack, DerydocaEngine::Rendering::Material* const& material, DerydocaEngine::Rendering::Projection const& projection, Transform* const& projectionTransform) {}
	virtual void postRender() {}
	inline void setGameObject(GameObject* const& gameObject) { m_gameObject = gameObject; }
	inline GameObject* getGameObject() { return m_gameObject; }
	virtual void deserialize(YAML::Node const& compNode) { };

	template<typename T>
	inline T* getComponent()
	{
		// Get the game object that this component belongs to
		GameObject* gameObject = getGameObject();
		if (gameObject == nullptr)
		{
			return nullptr;
		}

		// Iterate through all of the components on this game object
		std::vector<GameComponent*> gameObjectComponents = getGameObject()->getComponents();
		for (std::vector<GameComponent*>::iterator it = gameObjectComponents.begin(); it != gameObjectComponents.end(); ++it)
		{
			// Attempt to cast this object to the type we want
			GameComponent* component = *it;
			T* attemptedCast = dynamic_cast<T*>(component);

			// If the cast worked, return the component
			if (attemptedCast != nullptr)
			{
				return attemptedCast;
			}
		}

		// If we are here, we didn't find the component and have nothing to return
		return nullptr;
	}

protected:
	template<typename T>
	inline T getResource(YAML::Node const& node, std::string const& resourceName)
	{
		Resource* resource = getResource(node, resourceName);
		return static_cast<T>(resource);
	}

	inline Resource* getResource(YAML::Node const& node, std::string const& resourceName)
	{
		YAML::Node resourceNode = node[resourceName];

		if (resourceNode == nullptr)
		{
			return nullptr;
		}

		boost::uuids::uuid id = resourceNode.as<boost::uuids::uuid>();
		Resource * resource = ObjectLibrary::getInstance().getResource(id);
		return resource;
	}

	template<typename T>
	inline T* getResourceObject(boost::uuids::uuid const& resourceId)
	{
		return ObjectLibrary::getInstance().getResourceObject<T>(resourceId);
	}

	template<typename T>
	inline T* getResourceObject(YAML::Node const& node, std::string const& resourceName)
	{
		YAML::Node resourceIdNode = node[resourceName];
		if (resourceIdNode == nullptr || !resourceIdNode.IsScalar())
		{
			return nullptr;
		}

		boost::uuids::uuid resourceId = resourceIdNode.as<boost::uuids::uuid>();

		return getResourceObject<T>(resourceId);
	}

	template<typename T>
	inline std::vector<T> loadComponents(YAML::Node const& node, std::string const& componentCollectionName)
	{
		std::vector<T> objectArr = std::vector<T>();

		// Get the collection node
		YAML::Node componentIdCollectionNode = node[componentCollectionName];
		if (componentIdCollectionNode == nullptr || !componentIdCollectionNode.IsSequence())
		{
			return objectArr;
		}

		// Iterate through all component IDs in the collection
		for (size_t componentIndex = 0; componentIndex < componentIdCollectionNode.size(); componentIndex++)
		{
			boost::uuids::uuid id = componentIdCollectionNode[componentIndex].as<boost::uuids::uuid>();
			GameComponent* component = ObjectLibrary::getInstance().getComponent(id);
			if (component)
			{
				// Add it to the array if we found one
				objectArr.push_back((T)component);
			}
			else
			{
				// If no component was found, log the issue and continue on
				std::cout << "Unable to load the component with ID of '" << id << "' because it was not found in the ObjectLibrary.\n";
			}
		}

		// Serve up what we found
		return objectArr;
	}

private:
	GameObject* m_gameObject;
};
