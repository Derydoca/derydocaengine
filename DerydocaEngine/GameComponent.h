#pragma once

#include "yaml-cpp\yaml.h"
#include "YamlTools.h"
#include "ObjectLibrary.h"
#include "boost\uuid\uuid.hpp"

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
	virtual void update(float deltaTime) {}
	virtual void preRender() {}
	virtual void render(MatrixStack* matrixStack) {}
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
			T* attemptedCast = static_cast<T*>(component);

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
	inline T loadResource(YAML::Node node, std::string resourceName)
	{
		boost::uuids::uuid id = node[resourceName].as<boost::uuids::uuid>();
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

private:
	GameObject* m_gameObject;
};
