#pragma once
#include <list>
#include <memory>

#include "Scenes\Scene.h"
#include "Rendering\RenderTexture.h"

namespace DerydocaEngine {
	class GameObject;
	namespace Components {
		class Camera;
	}
}

namespace DerydocaEngine::Rendering
{

	class CameraManager
	{
	public:
		static CameraManager& getInstance() {
			static CameraManager instance;
			return instance;
		}

		inline std::list<std::shared_ptr<Components::Camera>> getCameras() { return m_cameras; }
		void addCamera(std::weak_ptr<Components::Camera> camera) { m_cameras.push_back(camera.lock()); }
		void removeCamera(std::weak_ptr<Components::Camera> camera) { m_cameras.remove(camera.lock()); }
		std::shared_ptr<Components::Camera> getCurrentCamera() { return m_currentCamera; }
		void setCurrentCamera(const std::shared_ptr<Components::Camera> camera) { m_currentCamera = camera; }
		void renderCamerasToAttachedRenderTextures(const std::vector<std::shared_ptr<Scenes::Scene>> scenes);

		void operator=(CameraManager const&) = delete;
	private:
		CameraManager();
		~CameraManager();

		CameraManager(CameraManager const&);

		std::list<std::shared_ptr<Components::Camera>> m_cameras;
		std::shared_ptr<Components::Camera> m_currentCamera;
	};

}
