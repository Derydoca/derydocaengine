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

	void CameraManager::render(std::shared_ptr<GameObject> const rootObject)
	{
		for each (Components::Camera* cam in m_cameras)
		{
			m_currentCamera = cam;
			cam->renderRoot(rootObject);
		}
	}

}
