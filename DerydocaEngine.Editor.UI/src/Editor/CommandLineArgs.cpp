#include "DerydocaEngine/Editor/CommandLineArgs.h"

namespace DerydocaEngine::Editor
{

	CommandLineArgs::CommandLineArgs(const int& argc, const char* argv[])
	{
		// Extract the path to the executable
		if (argc > 0)
		{
			m_ExecutablePath = argv[0];
		}

		// Iterate through all of the arguments
		std::string currentArgumentKey = "";
		for (int argIndex = 1; argIndex < argc; argIndex++)
		{
			std::string currentArgument = argv[argIndex];
			if (currentArgument[0] == '-')
			{
				// If the argument begins with a hyphen
				// Set the argument key to the current argument without the leading hyphen
				currentArgumentKey = currentArgument.substr(1);

				// Force the argument into the map
				m_Arguments[currentArgumentKey];
			}
			else
			{
				// If the argument does not start with a hyphen and we have determined an argument key
				if (currentArgumentKey != "")
				{
					// Add this argument to the current key's list of values
					m_Arguments[currentArgumentKey].push_back(currentArgument);
				}
			}
		}
	}

	const bool CommandLineArgs::KeyExists(const std::string& key)
	{
		return m_Arguments.find(key) != m_Arguments.end();
	}

	const std::vector<std::string> CommandLineArgs::GetValues(const std::string& key)
	{
		if (!KeyExists(key))
		{
			return std::vector<std::string>();
		}

		return m_Arguments[key];
	}

}
