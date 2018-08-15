#include "Editor.h"
#include "EngineSettings.h"
#include "ObjectLibrary.h"
#include "CommandLineArgs.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "FontFace.h"

using namespace std;

int main(int argc, char* argv[])
{
	CommandLineArgs args = CommandLineArgs(argc, argv);
	
	if (args.keyExists("processDirectory"))
	{
		vector<string> directoriesToProcess = args.getValues("processDirectory");

		for (auto it = directoriesToProcess.begin(); it != directoriesToProcess.end(); ++it)
		{
			ObjectLibrary::getInstance().updateMetaFilesDirectory(it->c_str());
		}

		return 0;
	}

	if (args.keyExists("processFont"))
	{
		vector<string> processArgs = args.getValues("processFont");
		if (processArgs.size() != 3)
		{
			cout << "Unable to process font file. Incorrect argument count." << endl;
			return 0;
		}

		FontFace face;
		face.loadFromFontFile(processArgs[0]);
		face.setFontSize((float)atof(processArgs[1].c_str()));
		face.saveToSerializedFile(processArgs[2]);
	}

	// Get the project path for the engine
	vector<string> projectArgs = args.getValues("project");
	if (projectArgs.size() == 0)
	{
		// If no project path was provided, the engine must exit
		cout << "No project was specified. Please supply the argument -project [MyProjectPath]" << endl;
		return 0;
	}

	// Get the path for the scene to load
	vector<string> sceneArgs = args.getValues("scene");
	string levelIdentifier;
	if (sceneArgs.size() > 0)
	{
		levelIdentifier = sceneArgs.front();
	}

	// Open the editor
	Editor* editor = new Editor();
	return editor->Run(projectArgs.front(), levelIdentifier);
	delete editor;
}