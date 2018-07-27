#pragma once

#include "yaml-cpp\yaml.h"
#include "YamlTools.h"
#include "ObjectLibrary.h"
#include "boost\uuid\uuid.hpp"
#include <vector>
#include "Material.h"
#include "Projection.h"

struct Resource;

class GameObject;
class MatrixStack;
class Camera;

#define GENINSTANCE(TYPE) \
	static GameComponent* generateInstance() { return new TYPE(); }

class GameComponent {
public:
	GameComponent() {}
	virtual void init() {}
	virtual void postInit() {}
	virtual void update(float deltaTime) {}
	virtual void preRender() {}
	virtual void render(MatrixStack* matrixStack) {}
	virtual void renderMesh(MatrixStack* matrixStack, Material* material, Projection projection, Transform* projectionTransform) {}
	virtual void postRender() {}
	inline void setGameObject(GameObject* gameObject) { m_gameObject = gameObject; }
	inline GameObject* getGameObject() { return m_gameObject; }
	virtual void deserialize(YAML::Node compNode) { };

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
	inline T getResource(YAML::Node node, std::string resourceName)
	{
		Resource* resource = getResource(node, resourceName);
		return static_cast<T>(resource);
	}

	inline Resource* getResource(YAML::Node node, std::string resourceName)
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
	inline T loadResource(YAML::Node node, std::string resourceName)
	{
		Resource * resource = getResource(node, resourceName);
		if (resource)
		{
			return (T)resource->getResourceObject();
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T>
	inline T loadResource(std::string resourceName)
	{
		boost::uuids::string_generator gen;
		uuid id = gen(resourceName);
		Resource * resource = ObjectLibrary::getInstance().getResource(id);
		if (resource)
		{
			return (T)resource->getResourceObject();
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T>
	inline std::vector<T> loadComponents(YAML::Node node, std::string componentCollectionName)
	{
		vector<T> objectArr = vector<T>();

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
				cout << "Unable to load the component with ID of '" << id << "' because it was not found in the ObjectLibrary." << endl;
			}
		}

		// Serve up what we found
		return objectArr;
	}

private:
	GameObject* m_gameObject;
};
