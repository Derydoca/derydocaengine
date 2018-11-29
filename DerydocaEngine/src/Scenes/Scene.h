#pragma once
#include <memory>

namespace DerydocaEngine {
	class GameObject;
}

namespace DerydocaEngine::Scenes
{

	class Scene {
	public:
		virtual ~Scene() {}
		virtual void setUp(std::shared_ptr<GameObject> const root) {}
		virtual void tearDown(std::shared_ptr<GameObject> const root) {}
	};

}
