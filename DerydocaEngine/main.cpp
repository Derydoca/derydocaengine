#include "Editor.h"
#include "EngineSettings.h"
#include "ObjectLibrary.h"
#include "CommandLineArgs.h"

int main(int argc, char* argv[])
{
	CommandLineArgs args = CommandLineArgs(argc, argv);
	
	if (args.keyExists("processDirectory"))
	{
		list<string> directoriesToProcess = args.getValues("processDirectory");

		for (auto it = directoriesToProcess.begin(); it != directoriesToProcess.end(); ++it)
		{
			ObjectLibrary::getInstance().initializeDirectory(it->c_str());
		}

		return 0;
	}
	else
	{
		// All other commands create the editor window
		Editor* editor = new Editor();
		return editor->Run();
	}
}