#pragma once
#include "Scenes\Scene.h"

namespace DerydocaEngine::Scenes
{

	class HardCodedScene : public Scene
	{
	public:
		HardCodedScene();
		~HardCodedScene();

		virtual void setUp() override;
		virtual void tearDown() override;

	};

}
