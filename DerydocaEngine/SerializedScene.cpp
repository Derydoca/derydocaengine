#include "SerializedScene.h"
#include "YamlTools.h"
#include <iostream>
#include <fstream>
#include "ObjectLibrary.h"

// Find a better (generic) way to initialize the scene objects
#include "Material.h"
#include "Shader.h"
#include "Mesh.h"
#include "MeshRenderer.h"

SerializedScene::SerializedScene()
{
}

SerializedScene::~SerializedScene()
{
}

void SerializedScene::setUp(GameObject * root, EngineSettings * settings, Display * display, Keyboard * keyboard, Mouse * mouse)
{
	// Initialize the components
	for (int i = 0; i < m_sceneObjects.size(); i++)
	{
		SceneObject* sceneObject = m_sceneObjects[i];
		if (sceneObject->isObjectCreated())
		{
			continue;
		}

		if (sceneObject->getType() == "Material")
		{
			YAML::Node properties = sceneObject->getProperties();
			std::string shaderPath = properties["Shader"].as<std::string>();
			Shader* shader = new Shader(shaderPath);
			Material* material = new Material();
			material->setShader(shader);
			YAML::Node parameters = properties["MaterialParameters"];
			for (int i = 0; i < parameters.size(); i++)
			{
				std::string paramType = parameters[i]["Type"].as<std::string>();
				if (paramType == "Texture")
				{
					int slot = parameters[i]["Slot"].as<int>();
					std::string texturePath = parameters[i]["Path"].as<std::string>();
					Texture* texture = new Texture(texturePath);
					material->setTextureSlot(slot, texture);
				}
			}
			sceneObject->setObjectReference(material);
		}
		else if (sceneObject->getType() == "Mesh")
		{
			std::string meshPath = sceneObject->getProperties()["Path"].as<std::string>();
			Mesh* mesh = new Mesh(meshPath);
			sceneObject->setObjectReference(mesh);
		}
		else if (sceneObject->getType() == "GameObject")
		{
			YAML::Node properties = sceneObject->getProperties();

			std::string name = properties["Name"].as<std::string>();
			GameObject* go = new GameObject(name);

			// Set the transform component
			Transform* trans = go->getTransform();
			YAML::Node transformNode = properties["Transform"];
			YAML::Node positionNode = transformNode["Position"];
			glm::vec3 Position = glm::vec3(positionNode[0].as<float>(), positionNode[1].as<float>(), positionNode[2].as<float>());
			trans->setPos(Position);
			YAML::Node rotationNode = transformNode["Rotation"];
			glm::vec3 rotationEuler = glm::vec3(rotationNode[0].as<float>(), rotationNode[1].as<float>(), rotationNode[2].as<float>());
			trans->setEulerAngles(rotationEuler);
			YAML::Node scaleNode = transformNode["Scale"];
			glm::vec3 scale = glm::vec3(scaleNode[0].as<float>(), scaleNode[1].as<float>(), scaleNode[2].as<float>());
			trans->setScale(scale);
			root->addChild(go);

			sceneObject->setObjectReference(go);
		}
	}

	// Resolve dependencies
	for (int i = 0; i < m_sceneObjects.size(); i++)
	{
		SceneObject* sceneObject = m_sceneObjects[i];
		if (!sceneObject->isObjectCreated())
		{
			continue;
		}

		if (sceneObject->getType() == "GameObject")
		{
			YAML::Node properties = sceneObject->getProperties();
			GameObject* go = (GameObject*)sceneObject->getObjectReference();

			YAML::Node componentNodes = properties["Components"];
			for (int componentIndex = 0; componentIndex < componentNodes.size(); componentIndex++)
			{
				YAML::Node compNode = componentNodes[componentIndex];
				std::string compType = compNode["Type"].as<std::string>();
				if (compType == "MeshRenderer")
				{
					int materialId = compNode["Material"].as<int>();
					SceneObject* meshObj = findNode(compNode["Mesh"].as<int>());
					if (!meshObj->isObjectCreated())
					{
						printf("Unable to create MeshRenderer component because material or mesh is invalid.");
						continue;
					}

					Resource* materialResource = ObjectLibrary::getInstance().getResource(materialId);
					Material* mat = (Material*)materialResource->getResourceObject();

					MeshRenderer* mr = new MeshRenderer((Mesh*)meshObj->getObjectReference(), mat);
					go->addComponent(mr);
				}
			}
		}
	}
}

void SerializedScene::tearDown(GameObject * root)
{
}

void SerializedScene::LoadFromFile(std::string filePath)
{
	printf("Loading scene from file: %s\n", filePath.c_str());
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
			printf("Skipping scene node %i because type is not defined.", i);
			continue;
		}

		if (!typeNode.IsScalar())
		{
			printf("Skipping scene node %i because type is not a scalar.", i);
			continue;
		}

		if (!typeNode.IsScalar())
		{
			printf("Skipping scene node %i because type is not a scalar.", i);
			continue;
		}

		if (!idNode || !idNode.IsScalar())
		{
			printf("Skipping scene node %i because it does not contain a valid ID.", i);
			continue;
		}

		if (!propertiesNode)
		{
			printf("Skipping scene node %i because it contains no properties.", i);
			continue;
		}

		int id = idNode.as<int>();
		std::string typeName = typeNode.as<std::string>();

		SceneObject* obj = new SceneObject(id, typeName, propertiesNode);

		// Store a tuple type object with object ID and generic pointer to object in memory then resolve all references
		m_sceneObjects.push_back(obj);
	}
}

void SerializedScene::SaveToFile(std::string filePath)
{
	YAML::Node root;
	
	YAML::Node materialNode;
	materialNode["Type"] = "Material";
	materialNode["ID"] = 100;
	YAML::Node materialProperties;
	materialProperties["Shader"] = "../res/diffuseFrag";
	YAML::Node materialTexture;
	materialTexture["Type"] = "Texture";
	materialTexture["Slot"] = 0;
	materialTexture["Path"] = "../res/grass.png";
	materialProperties["MaterialParameters"].push_back(materialTexture);
	materialNode["Properties"] = materialProperties;
	root["Scene"].push_back(materialNode);

	YAML::Node meshNode;
	meshNode["Type"] = "Mesh";
	meshNode["ID"] = 200;
	YAML::Node meshProperties;
	meshProperties["Path"] = "../res/sphere.obj";
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

SceneObject * SerializedScene::findNode(int id)
{
	for (int i = 0; i < m_sceneObjects.size(); i++)
	{
		if (m_sceneObjects[i]->getId() == id)
		{
			return m_sceneObjects[i];
		}
	}
	return nullptr;
}
