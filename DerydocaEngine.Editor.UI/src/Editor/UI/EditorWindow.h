#pragma once
#include <string>
#include "GameObject.h"
#include "Resources\Resource.h"
#include "Scenes\Scene.h"

namespace DerydocaEngine::Editor::UI
{

	class EditorWindow
	{
	public:
		int Run(
			const std::string& projectPath,
			const std::string& levelIdentifier
		);
	private:

		std::time_t m_levelLoadTime;
	};

}
