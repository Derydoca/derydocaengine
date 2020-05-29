#pragma once
#include <memory>

namespace DerydocaEngine {
	class GameObject;
}

namespace DerydocaEngine::Scenes
{

	class Scene {
	public:
		virtual void setUp() = 0;
		virtual void tearDown() = 0;
		virtual std::shared_ptr<GameObject> getRoot() const { return m_Root; }
	protected:
		std::shared_ptr<GameObject> m_Root;
	};

}
