#pragma once

namespace DerydocaEngine {
	class GameObject;
}

namespace DerydocaEngine::Scenes
{

	class Scene {
	public:
		virtual ~Scene() {}
		virtual void setUp(GameObject* const& root) {}
		virtual void tearDown(GameObject* const& root) {}
	};

}
