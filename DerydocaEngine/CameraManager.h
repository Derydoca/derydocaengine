#pragma once
#include <list>
#include "Camera.h"

class CameraManager
{
public:
	static CameraManager& getInstance() {
		static CameraManager instance;
		return instance;
	}

	inline std::list<DerydocaEngine::Components::Camera*> getCameras() { return m_cameras; }
	void addCamera(DerydocaEngine::Components::Camera* const& camera) { m_cameras.push_back(camera); }
	void removeCamera(DerydocaEngine::Components::Camera* const& camera) { m_cameras.remove(camera); }
	void render(GameObject* const& rootObject);
	DerydocaEngine::Components::Camera* getCurrentCamera() { return m_currentCamera; }

	void operator=(CameraManager const&) = delete;
private:
	CameraManager();
	~CameraManager();

	CameraManager(CameraManager const&);

	std::list<DerydocaEngine::Components::Camera*> m_cameras;
	DerydocaEngine::Components::Camera* m_currentCamera;
};
