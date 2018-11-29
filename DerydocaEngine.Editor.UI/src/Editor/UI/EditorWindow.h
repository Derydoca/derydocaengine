#pragma once
#include <string>
#include "GameObject.h"

namespace DerydocaEngine::Editor::UI
{

	class EditorWindow
	{
	public:
		int Run(const std::string& projectPath, const std::string& scenePath);
	private:
		std::shared_ptr<GameObject> m_sceneRoot;
	};

}
