#pragma once
#include <map>
#include <vector>
#include <string>

namespace DerydocaEngine::Editor::Execution
{

	using argumentCollection = std::map<std::string, std::vector<std::string>>;

	class CommandLineArgs
	{
	public:
		CommandLineArgs(int const& argc, char* argv[]);
		~CommandLineArgs();

		std::string getExecutablePath() const { return m_executablePath; }
		bool keyExists(std::string const& key);
		std::vector<std::string> getValues(std::string const& key);
	private:
		void processArguments(int const& argc, char* argv[]);

		std::string m_executablePath;
		argumentCollection m_arguments;
	};

}