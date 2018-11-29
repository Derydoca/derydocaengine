#pragma once
#include "Scenes\Scene.h"
#include "Scenes\SceneObject.h"

namespace DerydocaEngine::Scenes
{

	class SerializedScene : public Scene
	{
	public:
		SerializedScene();
		~SerializedScene();

		virtual void setUp(std::shared_ptr<GameObject> const root);
		virtual void tearDown(std::shared_ptr<GameObject> const root);

		void LoadFromFile(std::string const& filePath);
		void SaveToFile(std::string const& filePath);
	private:
		std::vector<SceneObject*> m_sceneObjects;

		SceneObject* findNode(boost::uuids::uuid const& id);
	};

}
