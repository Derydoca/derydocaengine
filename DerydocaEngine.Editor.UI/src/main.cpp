#include "EditorPch.h"
#include "Settings\EngineSettings.h"
#include "ObjectLibrary.h"
#include "Editor/Execution/CommandLineArgs.h"
#include <iostream>
#include "UI\FontFace.h"
#include "UI\Spritesheet.h"

#include "Scenes\SceneManager.h"
#include "Editor\EditorRenderer.h"
#include "Rendering\GraphicsAPI.h"
#include "Rendering\ShaderLibrary.h"
#include "SystemWindowingLayer.h"
#include "Resources\ResourceType.h"
#include "Files\FileUtils.h"

int main(int argc, char* argv[]) 
{
	DerydocaEngine::Logging::Log::Init();
	if (!DerydocaEngine::Settings::EngineSettings::Initialize())
	{
		DerydocaEngine::Logging::Log::GetCoreLogger()->critical("The engine settings failed to initialize!");
		return -1;
	};
	DerydocaEngine::SystemWindowingLayer::init();
	DerydocaEngine::SystemWindowingLayer::setVSync(true);
	DerydocaEngine::SystemWindowingLayer::setDepthTesting(true);
	DerydocaEngine::SystemWindowingLayer::setMultisampling(1, 16);

	// Parse the command line arguments
	DerydocaEngine::Editor::Execution::CommandLineArgs args = DerydocaEngine::Editor::Execution::CommandLineArgs(argc, argv);

	if (args.keyExists("processDirectory"))
	{
		std::vector<std::string> directoriesToProcess = args.getValues("processDirectory");

		for (auto it = directoriesToProcess.begin(); it != directoriesToProcess.end(); ++it)
		{
			DerydocaEngine::ObjectLibrary::getInstance().updateMetaFilesDirectory(it->c_str());
		}

		return 0;
	}

	if (args.keyExists("processFont"))
	{
		std::vector<std::string> processArgs = args.getValues("processFont");
		if (processArgs.size() != 3)
		{
			D_LOG_ERROR("Unable to process font file. Incorrect argument count.");
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
			D_LOG_ERROR("Unable to process sprite sheet. Incorrect argument count.");
			return 0;
		}

		// Load the settings
		auto settings = DerydocaEngine::Settings::EngineSettings::getInstance();

		// Load the project file
		DerydocaEngine::ObjectLibrary::getInstance().loadEngineResources(settings->getEngineResourceDirectory());
		DerydocaEngine::ObjectLibrary::getInstance().loadProjectResources(processArgs[0]);

		// Process the sprite sheet
		DerydocaEngine::UI::SpriteSheet ss;
		// SpriteSheet::LoadFromDisk was removed due to it's reliance on YAML serialization which is now removed
		//ss.LoadFromDisk(processArgs[1]);
		//ss.updateTexture();
		//ss.SaveToSerializedFile(processArgs[1]);
	}

	if (args.keyExists("upgrade"))
	{
		std::vector<std::string> processArgs = args.getValues("upgrade");
		if (processArgs.size() < 1)
		{
			D_LOG_ERROR("Unable to upgrade. Invalid args count.");
			return 0;
		}

		// Load the settings
		auto settings = DerydocaEngine::Settings::EngineSettings::getInstance();

		// Load the project file
		auto& ol = DerydocaEngine::ObjectLibrary::getInstance();
		for (size_t i = 0; i < processArgs.size(); i++)
		{
			ol.loadProjectResources(processArgs[i]);
		}

		// Get all material files
		auto materials = ol.getResourcesOfType(DerydocaEngine::Resources::ResourceType::MaterialResourceType);

		// Create the renderer
		auto renderer = DerydocaEngine::Rendering::Renderer(DerydocaEngine::Editor::EditorRenderer::GetInstance());
		renderer.init();

		// For each
		for (auto matRes : materials)
		{
			// Save them
			auto material = std::static_pointer_cast<DerydocaEngine::Rendering::Material>(matRes->getResourceObjectPointer());

			auto matData = material->ToData();
			DerydocaEngine::Files::Utils::WriteToDisk<DerydocaEngine::AssetData::MaterialData>(matData, matRes->getSourceFilePath());
		}

		// Exit
		return 0;
	}

	// Get information about the project specified in the command arguments
	std::vector<std::string> projectArgs = args.getValues("project");
	auto projectDirectory = projectArgs.size() > 0 ?
		boost::filesystem::path(projectArgs.front()) :
		boost::filesystem::path();
	bool projectDirectoryIsDefined = projectArgs.size() > 0;

	// Get information about the scene specified in the command arguments
	std::vector<std::string> sceneArgs = args.getValues("scene");
	bool sceneIdIsDefined = sceneArgs.size() > 0;
	boost::uuids::uuid sceneId;
	if (sceneArgs.size() > 0)
	{
		boost::uuids::string_generator stringGen;
		sceneId = stringGen(sceneArgs.front().c_str());
	}

	// Create the renderer
	auto renderer = DerydocaEngine::Rendering::Renderer(DerydocaEngine::Editor::EditorRenderer::GetInstance());

	// Load the settings
	auto engineSettings = DerydocaEngine::Settings::EngineSettings::getInstance();

	// Load the engine's resources
	DerydocaEngine::ObjectLibrary::getInstance().loadEngineResources(engineSettings->getEngineResourceDirectory());

	// Load the project directory
	if (projectDirectoryIsDefined)
	{
		if (boost::filesystem::exists(projectDirectory))
		{
			// Load the project file
			DerydocaEngine::ObjectLibrary::getInstance().loadProjectResources(projectDirectory);
		}
		else
		{
			D_LOG_ERROR("Invalid project directory: {}", projectDirectory.string());
		}
	}
	else
	{
		D_LOG_ERROR("No project was specified. Please supply the argument -project [MyProjectPath]");
	}

	DerydocaEngine::Rendering::ShaderLibrary::getInstance().initialize();
	renderer.init();

	// Load a scene if it is defined
	if (sceneIdIsDefined)
	{
		DerydocaEngine::Scenes::SceneManager::getInstance().LoadScene(sceneId);
	}

	// Start the render loop
	int result = renderer.runRenderLoop();

	engineSettings->Save();

	return result;
}

#ifdef _WINDOWS
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
{
	return main(__argc, __argv);
}
#endif
