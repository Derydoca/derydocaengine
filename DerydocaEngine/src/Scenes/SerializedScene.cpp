#include "Scenes\SerializedScene.h"
#include <iostream>
#include <fstream>
#include "ObjectLibrary.h"
#include "Helpers\YamlTools.h"
#include "Components\GameComponentFactory.h"
#include <iostream>
#include "Components\GameComponent.h"

namespace DerydocaEngine::Scenes
{

	SerializedScene::SerializedScene()
	{
	}

	SerializedScene::~SerializedScene()
	{
	}

	void SerializedScene::setUp(std::shared_ptr<GameObject> const root)
	{
		// Initialize the components
		for (size_t i = 0; i < m_sceneObjects.size(); i++)
		{
			SceneObject* sceneObject = m_sceneObjects[i];

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
			std::shared_ptr<GameObject> go = std::make_shared<GameObject>(name);

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
			trans->setPos(transformPosition);
			trans->setQuat(transformQuat);
			trans->setScale(transformScale);

			sceneObject->setObjectReference(go);
		}

		// Resolve dependencies
		for (size_t i = 0; i < m_sceneObjects.size(); i++)
		{
			SceneObject* sceneObject = m_sceneObjects[i];

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
				SceneObject* parentSceneObject = findNode(parentId);
				std::shared_ptr<GameObject> parentGo = parentSceneObject->getGameObject();
				parentGo->addChild(go);
			}
			else
			{
				// Otherwise this object should exist on the root of the scene
				root->addChild(go);
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

				// Let the component deserialize the data it ineeds
				if (compNode["Properties"])
				{
					component->deserialize(compNode["Properties"]);
				}

				// Add the component to the game object
				go->addComponent(component);

				YAML::Node componentIdNode = compNode["ID"];
				if (componentIdNode)
				{
					ObjectLibrary::getInstance().registerComponent(componentIdNode.as<boost::uuids::uuid>(), component);
				}
			}
		}
	}

	void SerializedScene::tearDown(std::shared_ptr<GameObject> const root)
	{
	}

	void SerializedScene::LoadFromFile(std::string const& filePath)
	{
		std::cout << "Loading scene: " << filePath << "\n";
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
				std::cout << "Skipping scene node " << i << " because type is not defined.\n";
				continue;
			}

			if (!typeNode.IsScalar())
			{
				std::cout << "Skipping scene node " << i << " because type is not a scalar.\n";
				continue;
			}

			if (!typeNode.IsScalar())
			{
				std::cout << "Skipping scene node " << i << " because type is not a scalar.\n";
				continue;
			}

			if (!idNode || !idNode.IsScalar())
			{
				std::cout << "Skipping scene node " << i << " because it does not contain a valid ID.\n";
				continue;
			}

			if (!propertiesNode)
			{
				std::cout << "Skipping scene node " << i << " because it contains no properties.\n";
				continue;
			}

			boost::uuids::uuid id = idNode.as<boost::uuids::uuid>();
			std::string typeName = typeNode.as<std::string>();

			SceneObject* obj = new SceneObject(id, typeName, propertiesNode);

			// Store a tuple type object with object ID and generic pointer to object in memory then resolve all references
			m_sceneObjects.push_back(obj);
		}
	}

	void SerializedScene::SaveToFile(std::string const& filePath)
	{
		YAML::Node root;

		YAML::Node materialNode;
		materialNode["Type"] = "Material";
		materialNode["ID"] = 100;
		YAML::Node materialProperties;
		materialProperties["Shader"] = "..\\engineResources\\shaders\\diffuseFrag";
		YAML::Node materialTexture;
		materialTexture["Type"] = "Texture";
		materialTexture["Slot"] = 0;
		materialTexture["Path"] = ".\\engineResources\\grass.png";
		materialProperties["MaterialParameters"].push_back(materialTexture);
		materialNode["Properties"] = materialProperties;
		root["Scene"].push_back(materialNode);

		YAML::Node meshNode;
		meshNode["Type"] = "Mesh";
		meshNode["ID"] = 200;
		YAML::Node meshProperties;
		meshProperties["Path"] = ".\\engineResources\\sphere.obj";
		meshNode["Properties"] = meshProperties;
		root["Scene"].push_back(meshNode);

		YAML::Node gameObjectNode;
		gameObjectNode["Type"] = "GameObject";
		gameObjectNode["ID"] = 300;
		YAML::Node gameObjectProperties;
		gameObjectProperties["Name"] = "Diffuse Shaded Model";
		YAML::Node gameObjectTransform;
		gameObjectTransform["Position"].push_back<float>(0);
		gameObjectTransform["Position"].push_back<float>(0);
		gameObjectTransform["Position"].push_back<float>(-1);
		gameObjectTransform["Rotation"].push_back<float>(0);
		gameObjectTransform["Rotation"].push_back<float>(0);
		gameObjectTransform["Rotation"].push_back<float>(0);
		gameObjectTransform["Scale"].push_back<float>(1);
		gameObjectTransform["Scale"].push_back<float>(1);
		gameObjectTransform["Scale"].push_back<float>(1);
		gameObjectProperties["Transform"] = gameObjectTransform;
		YAML::Node meshRendererComponent;
		meshRendererComponent["Type"] = "MeshRenderer";
		meshRendererComponent["ID"] = 310;
		meshRendererComponent["Material"] = 100;
		meshRendererComponent["Mesh"] = 200;
		gameObjectProperties["Components"].push_back(meshRendererComponent);
		gameObjectNode["Properties"] = gameObjectProperties;
		root["Scene"].push_back(gameObjectNode);

		YAML::Emitter out;
		out.SetIndent(2);
		out.SetMapFormat(YAML::Block);
		out << root;
		//out.c_str()
		std::ofstream file;
		file.open(filePath);
		file << out.c_str();
		file.close();
	}

	SceneObject * SerializedScene::findNode(boost::uuids::uuid const& id)
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
