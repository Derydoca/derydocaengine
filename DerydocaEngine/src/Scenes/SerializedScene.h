#pragma once
#include "Scenes\Scene.h"

namespace DerydocaEngine::Scenes
{

	class SerializedScene : public Scene
	{
	public:
		SerializedScene();
		~SerializedScene();

		virtual void setUp();
		virtual void tearDown();

		SERIALIZE_FUNC_BASE(DerydocaEngine::Scenes::Scene);
	};

}
