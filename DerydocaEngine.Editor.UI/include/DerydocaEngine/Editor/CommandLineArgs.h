#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace DerydocaEngine::Editor
{
	using ArgumentCollection = std::unordered_map<std::string, std::vector<std::string>>;

	class CommandLineArgs
	{
	public:
		CommandLineArgs(const int& argc, const char* argv[]);

		const std::string GetExecutablePath() const { return m_ExecutablePath; }
		const std::vector<std::string> GetValues(const std::string& key);
		const bool KeyExists(const std::string& key);

	private:
		std::string m_ExecutablePath;
		ArgumentCollection m_Arguments;

	};

}