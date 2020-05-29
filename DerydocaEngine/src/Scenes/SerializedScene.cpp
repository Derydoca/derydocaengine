#include "EnginePch.h"
#include "Scenes\SerializedScene.h"
#include <iostream>
#include <fstream>
#include "ObjectLibrary.h"
#include "Helpers\YamlTools.h"
#include "Components\GameComponentFactory.h"
#include <iostream>
#include "Components\GameComponent.h"
#include <fstream>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include "Files\FileUtils.h"

namespace DerydocaEngine::Scenes
{

	SerializedScene::SerializedScene() :
		m_sceneObjects()
	{
	}

	SerializedScene::~SerializedScene()
	{
	}

	void SerializedScene::setUp()
	{
		boost::uuids::basic_random_generator<boost::mt19937> uuidGenerator;

		m_Root = std::make_shared<GameObject>("__SCENE_ROOT__");

		// Initialize the components
		for (size_t i = 0; i < m_sceneObjects.size(); i++)
		{
			auto sceneObject = m_sceneObjects[i];

			// If the object is already created, move onto the next object
			if (sceneObject->isObjectCreated())
			{
				continue;
			}

			YAML::Node properties = sceneObject->getProperties();

			// Get the name of the game object
			std::string name;
			if (properties["Name"])
			{
				name = properties["Name"].as<std::string>();
			}
			else
			{
				name = "[NO NAME]";
			}

			// Create the game object with the name found in the file
			std::shared_ptr<GameObject> go = std::make_shared<GameObject>(sceneObject->getId(), name);

			// Set the transform component
			std::shared_ptr<Components::Transform> trans = go->getTransform();
			YAML::Node transformNode = properties["Transform"];

			// Get the position
			glm::vec3 transformPosition;
			if (transformNode["Position"])
			{
				transformPosition = transformNode["Position"].as<glm::vec3>();
			}

			// Get the rotation as a quaternion
			glm::fquat transformQuat;
			if (transformNode["Rotation"])
			{
				transformQuat = transformNode["Rotation"].as<glm::fquat>();
			}

			// Get the scale
			glm::vec3 transformScale;
			if (transformNode["Scale"])
			{
				transformScale = transformNode["Scale"].as<glm::vec3>();
			}

			// Apply the transform, rotation (quat), and sclae
			trans->setPosition(transformPosition);
			trans->setQuaternion(transformQuat);
			trans->setScale(transformScale);

			sceneObject->setObjectReference(go);
		}

		// Resolve dependencies
		for (size_t i = 0; i < m_sceneObjects.size(); i++)
		{
			auto sceneObject = m_sceneObjects[i];

			// Do not resolve the object if it is not created for some reason
			if (!sceneObject->isObjectCreated())
			{
				continue;
			}

			YAML::Node properties = sceneObject->getProperties();
			std::shared_ptr<GameObject> go = sceneObject->getGameObject();

			// Determine if this game object is parented to another game object
			YAML::Node parentObjectIdNode = properties["Parent"];
			if (parentObjectIdNode)
			{
				// If it is parented to another game object, set the relationship
				boost::uuids::uuid parentId = parentObjectIdNode.as<boost::uuids::uuid>();
				auto parentSceneObject = findNode(parentId);
				std::shared_ptr<GameObject> parentGo = parentSceneObject->getGameObject();
				parentGo->addChild(go);
			}
			else
			{
				// Otherwise this object should exist on the root of the scene
				m_Root->addChild(go);
			}

			// Iterate through each component
			YAML::Node componentNodes = properties["Components"];
			for (size_t componentIndex = 0; componentIndex < componentNodes.size(); componentIndex++)
			{
				YAML::Node compNode = componentNodes[componentIndex];
				std::string compType = compNode["Type"].as<std::string>();

				// Create a game component based on the component type provided
				auto component = Components::GameComponentFactory::getInstance().generate(compType);

				// If no component was created, the component type is not supported so we should continue
				if (component == nullptr)
				{
					continue;
				}

				// Add the component to the game object
				go->addComponent(component);

				// Let the component deserialize the data it needs
				if (compNode["Properties"])
				{
					component->deserialize(compNode["Properties"]);
				}

				// Set the component's ID
				YAML::Node componentIdNode = compNode["ID"];
				boost::uuids::uuid componentId = boost::uuids::uuid();
				componentId = componentIdNode ? componentIdNode.as<boost::uuids::uuid>() : uuidGenerator();
				component->setId(componentId);
			}
		}
		// HACK: Forcing this vector to clear so that game object references arent being held onto unnecessarily
		m_sceneObjects.clear();
	}

	void SerializedScene::tearDown()
	{
		m_sceneObjects.clear();
		m_Root->preDestroy();
		m_Root = nullptr;
	}

	void SerializedScene::LoadFromFile(const std::string& filePath)
	{
		D_LOG_TRACE("Loading scene: {}", filePath);
		YAML::Node file = YAML::LoadFile(filePath);
		YAML::Node scene = file["Scene"];
		for (unsigned i = 0; i < scene.size(); i++)
		{
			YAML::Node sceneNode = scene[i];
			YAML::Node typeNode = sceneNode["Type"];
			YAML::Node idNode = sceneNode["ID"];
			YAML::Node propertiesNode = sceneNode["Properties"];

			if (!typeNode)
			{
				D_LOG_WARN("Skipping scene node {} because type is not defined.", i);
				continue;
			}

			if (!typeNode.IsScalar())
			{
				D_LOG_WARN("Skipping scene node {} because type is not a scalar.", i);
				continue;
			}

			if (!idNode || !idNode.IsScalar())
			{
				D_LOG_WARN("Skipping scene node {} because it does not contain a valid ID.", i);
				continue;
			}

			if (!propertiesNode)
			{
				D_LOG_WARN("Skipping scene node {} because it contains no properties.", i);
				continue;
			}

			boost::uuids::uuid id = idNode.as<boost::uuids::uuid>();
			std::string typeName = typeNode.as<std::string>();

			auto obj = std::make_shared<SceneObject>(id, typeName, propertiesNode);

			// Store a tuple type object with object ID and generic pointer to object in memory then resolve all references
			m_sceneObjects.push_back(obj);
		}
	}

	void SerializedScene::SaveToFile(const std::string& filePath)
	{
		Files::Utils::WriteToDisk(*this, filePath);
	}

	std::shared_ptr<SceneObject> SerializedScene::findNode(const boost::uuids::uuid& id)
	{
		for (size_t i = 0; i < m_sceneObjects.size(); i++)
		{
			if (m_sceneObjects[i]->getId() == id)
			{
				return m_sceneObjects[i];
			}
		}
		return nullptr;
	}

}
