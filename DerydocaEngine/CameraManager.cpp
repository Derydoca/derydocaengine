#include "CameraManager.h"
#include "GameObject.h"

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
}

void CameraManager::render(GameObject* rootObject)
{
	for each (Camera* cam in m_cameras)
	{
		m_currentCamera = cam;
		cam->renderRoot(rootObject);
	}
}
