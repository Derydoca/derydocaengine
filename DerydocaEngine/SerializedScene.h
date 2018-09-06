#pragma once
#include "Scene.h"
#include "SceneObject.h"

class SerializedScene : public Scene
{
public:
	SerializedScene();
	~SerializedScene();

	void setUp(GameObject* const& root);
	void tearDown(GameObject* const& root);

	void LoadFromFile(std::string const& filePath);
	void SaveToFile(std::string const& filePath);
private:
	std::vector<SceneObject*> m_sceneObjects;

	SceneObject* findNode(boost::uuids::uuid const& id);
};

