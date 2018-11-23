#pragma once
#include <string>

namespace DerydocaEngine::Execution
{

	class Editor
	{
	public:
		int Run(std::string const& projectPath, std::string const& scenePath);
	};

}
