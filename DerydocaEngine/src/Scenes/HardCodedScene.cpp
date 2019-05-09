#include "EnginePch.h"
#include "HardCodedScene.h"

DerydocaEngine::Scenes::HardCodedScene::HardCodedScene()
{
}

DerydocaEngine::Scenes::HardCodedScene::~HardCodedScene()
{
}

void DerydocaEngine::Scenes::HardCodedScene::setUp()
{
	m_root = std::make_shared<GameObject>("__root__");
	m_root->init();
	m_root->postInit();
}

void DerydocaEngine::Scenes::HardCodedScene::tearDown()
{
	m_root->preDestroy();
}
