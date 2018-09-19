#pragma once
#include <yaml-cpp\yaml.h>
#include <boost\uuid\uuid.hpp>
#include "GameObject.h"

namespace DerydocaEngine::Scenes
{

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
		DerydocaEngine::GameObject* getGameObject() const { return m_objectReference; }
		void setObjectReference(DerydocaEngine::GameObject* const& reference) { m_objectReference = reference; }
	private:
		boost::uuids::uuid m_id;
		std::string m_type;
		YAML::Node m_properties;
		DerydocaEngine::GameObject* m_objectReference;
	};

}
