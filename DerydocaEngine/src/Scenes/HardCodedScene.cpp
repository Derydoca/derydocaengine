#include "EnginePch.h"
#include "HardCodedScene.h"
#include "GameObject.h"

DerydocaEngine::Scenes::HardCodedScene::HardCodedScene()
{
}

DerydocaEngine::Scenes::HardCodedScene::~HardCodedScene()
{
}

void DerydocaEngine::Scenes::HardCodedScene::setUp()
{
	m_Root = std::make_shared<GameObject>("__root__");
	m_Root->init();
	m_Root->postInit();
}

void DerydocaEngine::Scenes::HardCodedScene::tearDown()
{
	m_Root->preDestroy();
}
