#pragma once
#include <yaml-cpp\yaml.h>
#include "GameObject.h"

class SceneObject
{
public:
	SceneObject(boost::uuids::uuid const& id, std::string const& type, YAML::Node const& propertiesNode) :
		m_id(id),
		m_type(type),
		m_properties(propertiesNode)
	{
	}

	~SceneObject() {}

	boost::uuids::uuid getId() const { return m_id; }
	std::string getType() const { return m_type; }
	YAML::Node getProperties() const { return m_properties; }
	bool isObjectCreated() const { return m_objectReference != 0; }
	GameObject* getGameObject() const { return m_objectReference; }
	void setObjectReference(GameObject* const& reference) { m_objectReference = reference; }
private:
	boost::uuids::uuid m_id;
	std::string m_type;
	YAML::Node m_properties;
	GameObject* m_objectReference;
};
