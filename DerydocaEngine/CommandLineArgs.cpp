#include "CommandLineArgs.h"



CommandLineArgs::CommandLineArgs(int argc, char* argv[])
{
	processArguments(argc, argv);
}


CommandLineArgs::~CommandLineArgs()
{
}

void CommandLineArgs::processArguments(int argc, char * argv[])
{
	// Extract the path to the executable
	if (argc > 0)
	{
		m_executablePath = argv[0];
	}

	// Iterate through all of the arguments
	string currentArgumentKey = "";
	for (int argIndex = 1; argIndex < argc; argIndex++)
	{
		string currentArgument = argv[argIndex];
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

bool CommandLineArgs::keyExists(string key)
{
	return m_arguments.find(key) != m_arguments.end();
}

list<string> CommandLineArgs::getValues(string key)
{
	if (!keyExists(key))
	{
		return list<string>();
	}

	return m_arguments[key];
}
