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

		inline std::list<Components::Camera*> getCameras() { return m_cameras; }
		void addCamera(Components::Camera* const& camera) { m_cameras.push_back(camera); }
		void removeCamera(Components::Camera* const& camera) { m_cameras.remove(camera); }
		void render(const std::vector<std::shared_ptr<Scenes::Scene>> scenes);
		void render(const std::vector<std::shared_ptr<Scenes::Scene>> scenes, std::shared_ptr<RenderTexture> renderTexture);
		Components::Camera* getCurrentCamera() { return m_currentCamera; }

		void operator=(CameraManager const&) = delete;
	private:
		CameraManager();
		~CameraManager();

		CameraManager(CameraManager const&);

		std::list<Components::Camera*> m_cameras;
		Components::Camera* m_currentCamera;
	};

}
