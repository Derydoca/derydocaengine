#include "EnginePch.h"
#include "Rendering\CameraManager.h"

#include "Components\Camera.h"
#include "GameObject.h"

namespace DerydocaEngine::Rendering
{

	CameraManager::CameraManager()
	{
	}

	CameraManager::~CameraManager()
	{
	}

	void CameraManager::render(const std::vector<std::shared_ptr<Scenes::Scene>> scenes)
	{
		for each (auto cam in m_cameras)
		{
			m_currentCamera = cam;

			cam->renderScenes(scenes);
		}
	}

}
