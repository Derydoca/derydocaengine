#include "Editor.h"
#include "EngineSettings.h"
#include "ObjectLibrary.h"
#include "CommandLineArgs.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	CommandLineArgs args = CommandLineArgs(argc, argv);
	
	if (args.keyExists("processDirectory"))
	{
		list<string> directoriesToProcess = args.getValues("processDirectory");

		for (auto it = directoriesToProcess.begin(); it != directoriesToProcess.end(); ++it)
		{
			ObjectLibrary::getInstance().updateMetaFilesDirectory(it->c_str());
		}

		return 0;
	}

	// Get the project path for the engine
	list<string> projectArgs = args.getValues("project");
	if (projectArgs.size() == 0)
	{
		// If no project path was provided, the engine must exit
		cout << "No project was specified. Please supply the argument -project [MyProjectPath]" << endl;
		return 0;
	}

	// Get the path for the scene to load
	list<string> sceneArgs = args.getValues("scene");
	string levelIdentifier;
	if (sceneArgs.size() > 0)
	{
		levelIdentifier = sceneArgs.front();
	}

	// Open the editor
	Editor* editor = new Editor();
	return editor->Run(projectArgs.front(), levelIdentifier);
}