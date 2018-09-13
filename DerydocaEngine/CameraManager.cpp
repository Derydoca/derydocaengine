#include "CameraManager.h"
#include "GameObject.h"

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
}

void CameraManager::render(GameObject* const& rootObject)
{
	for each (DerydocaEngine::Components::Camera* cam in m_cameras)
	{
		m_currentCamera = cam;
		cam->renderRoot(rootObject);
	}
}
