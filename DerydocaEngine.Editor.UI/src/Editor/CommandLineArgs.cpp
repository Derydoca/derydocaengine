#include "CommandLineArgs.h"

namespace DerydocaEngine::Editor
{

	CommandLineArgs::CommandLineArgs(int const& argc, char* argv[])
	{
		processArguments(argc, argv);
	}

	CommandLineArgs::~CommandLineArgs()
	{
	}

	void CommandLineArgs::processArguments(int const& argc, char * argv[])
	{
		// Extract the path to the executable
		if (argc > 0)
		{
			m_executablePath = argv[0];
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
				m_arguments[currentArgumentKey];
			}
			else
			{
				// If the argument does not start with a hyphen and we have determined an argument key
				if (currentArgumentKey != "")
				{
					// Add this argument to the current key's list of values
					m_arguments[currentArgumentKey].push_back(currentArgument);
				}
			}
		}
	}

	bool CommandLineArgs::keyExists(std::string const& key)
	{
		return m_arguments.find(key) != m_arguments.end();
	}

	std::vector<std::string> CommandLineArgs::getValues(std::string const& key)
	{
		if (!keyExists(key))
		{
			return std::vector<std::string>();
		}

		return m_arguments[key];
	}

}
