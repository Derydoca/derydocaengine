#include "EnginePch.h"
#include "HardCodedScene.h"

DerydocaEngine::Scenes::HardCodedScene::HardCodedScene()
{
	m_root = std::make_shared<GameObject>("__root__");
	m_root->init();
	m_root->postInit();
}

DerydocaEngine::Scenes::HardCodedScene::~HardCodedScene()
{
	m_root->preDestroy();
}

void DerydocaEngine::Scenes::HardCodedScene::setUp()
{
}

void DerydocaEngine::Scenes::HardCodedScene::tearDown()
{
}
