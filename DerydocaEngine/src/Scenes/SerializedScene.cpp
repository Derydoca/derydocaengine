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

	SerializedScene::SerializedScene()
	{
	}

	SerializedScene::~SerializedScene()
	{
	}

	void SerializedScene::setUp()
	{
		if (!m_Root)
		{
			m_Root = std::make_shared<GameObject>("__SCENE_ROOT__");
		}
	}

	void SerializedScene::tearDown()
	{
		m_Root->preDestroy();
		m_Root = nullptr;
	}

}
