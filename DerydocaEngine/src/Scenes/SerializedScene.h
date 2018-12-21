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

		virtual void setUp();
		virtual void tearDown();

		void LoadFromFile(std::string const& filePath);
		void SaveToFile(std::string const& filePath);
	private:
		std::vector<SceneObject*> m_sceneObjects;

		SceneObject* findNode(boost::uuids::uuid const& id);
	};

}
