#pragma once
#include <string>

namespace DerydocaEngine::Editor::UI
{

	class EditorWindow
	{
	public:
		int Run(std::string const& projectPath, std::string const& scenePath);
	};

}
