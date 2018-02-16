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
	virtual void render(MatrixStack* matrixStack) {}
	virtual void postRender() {}
	inline void setGameObject(GameObject* gameObject) { m_gameObject = gameObject; }
	inline GameObject* getGameObject() { return m_gameObject; }
	virtual void deserialize(YAML::Node compNode) { };

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
