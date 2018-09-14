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
#include "SpriteSheet.h"

int main(int argc, char* argv[])
{
	DerydocaEngine::Execution::CommandLineArgs args = DerydocaEngine::Execution::CommandLineArgs(argc, argv);
	
	if (args.keyExists("processDirectory"))
	{
		std::vector<std::string> directoriesToProcess = args.getValues("processDirectory");

		for (auto it = directoriesToProcess.begin(); it != directoriesToProcess.end(); ++it)
		{
			ObjectLibrary::getInstance().updateMetaFilesDirectory(it->c_str());
		}

		return 0;
	}

	if (args.keyExists("processFont"))
	{
		std::vector<std::string> processArgs = args.getValues("processFont");
		if (processArgs.size() != 3)
		{
			std::cout << "Unable to process font file. Incorrect argument count.\n";
			return 0;
		}

		DerydocaEngine::UI::FontFace face;
		face.loadFromFontFile(processArgs[0]);
		face.setFontSize((float)atof(processArgs[1].c_str()));
		face.saveToSerializedFile(processArgs[2]);
	}

	if (args.keyExists("processSpriteSheet"))
	{
		std::vector<std::string> processArgs = args.getValues("processSpriteSheet");
		if (processArgs.size() != 2)
		{
			std::cout << "Unable to process sprite sheet. Incorrect argument count.\n";
			return 0;
		}

		EngineSettings* settings = new EngineSettings(".\\engineSettings.yaml");

		// Load the project file
		ObjectLibrary::getInstance().initialize(settings->getEngineResourceDirectory(), processArgs[0]);

		// Process the sprite sheet
		SpriteSheet ss;
		ss.LoadFromDisk(processArgs[1]);
		ss.updateTexture();
		ss.saveToDisk(processArgs[1]);
	}

	// Get the project path for the engine
	std::vector<std::string> projectArgs = args.getValues("project");
	if (projectArgs.size() == 0)
	{
		// If no project path was provided, the engine must exit
		std::cout << "No project was specified. Please supply the argument -project [MyProjectPath]\n";
		return 0;
	}

	// Get the path for the scene to load
	std::vector<std::string> sceneArgs = args.getValues("scene");
	std::string levelIdentifier;
	if (sceneArgs.size() > 0)
	{
		levelIdentifier = sceneArgs.front();
	}

	// Open the editor
	DerydocaEngine::Execution::Editor* editor = new DerydocaEngine::Execution::Editor();
	return editor->Run(projectArgs.front(), levelIdentifier);
	delete editor;
}