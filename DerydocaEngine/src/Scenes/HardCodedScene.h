#pragma once
#include "Scenes\Scene.h"
#include "Scenes\SceneObject.h"

namespace DerydocaEngine::Scenes
{

	class HardCodedScene : public Scene
	{
	public:
		HardCodedScene();
		~HardCodedScene();

		virtual void setUp() override;
		virtual void tearDown() override;

	private:
		std::vector<std::shared_ptr<SceneObject>> m_sceneObjects;

	};

}
