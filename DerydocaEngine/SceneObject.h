#pragma once
#include <yaml-cpp\yaml.h>

class SceneObject
{
public:
	SceneObject(int id, std::string type, YAML::Node propertiesNode) :
		m_id(id),
		m_type(type),
		m_properties(propertiesNode)
	{
	}

	~SceneObject() {}

	int getId() { return m_id; }
	std::string getType() { return m_type; }
	YAML::Node getProperties() { return m_properties; }
	bool isObjectCreated() { return m_objectReference != 0; }
	void* getObjectReference() { return m_objectReference; }
	void setObjectReference(void* reference) { m_objectReference = reference; }
private:
	int m_id;
	std::string m_type;
	YAML::Node m_properties;
	void* m_objectReference;
};
