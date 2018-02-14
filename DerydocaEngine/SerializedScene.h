#pragma once
#include "Scene.h"
#include "SceneObject.h"

class SerializedScene : public Scene
{
public:
	SerializedScene();
	~SerializedScene();

	void setUp(GameObject* root);
	void tearDown(GameObject* root);

	void LoadFromFile(std::string filePath);
	void SaveToFile(std::string filePath);
private:
	std::vector<SceneObject*> m_sceneObjects;

	SceneObject* findNode(boost::uuids::uuid id);
};

