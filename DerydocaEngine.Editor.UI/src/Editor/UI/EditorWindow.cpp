#include "EditorPch.h"
#include "Editor\UI\EditorWindow.h"

#include <boost\filesystem.hpp>
#include <iostream>
#include <sdl2\SDL.h>

#include "Input\InputManager.h"
#include "ObjectLibrary.h"
#include "Rendering\CameraManager.h"
#include "Rendering\Display.h"
#include "Scenes\SceneManager.h"
#include "Scenes\SerializedScene.h"
#include "Settings\EngineSettings.h"
#include "Timing\Clock.h"

namespace DerydocaEngine::Editor::UI
{

	std::shared_ptr<Resources::Resource> getLevelResource(const std::string& levelIdentifier, const std::string& levelType)
	{
		// Load the editor components
		if (levelIdentifier.empty())
		{
			std::cout << "No editor components level identifier was provided!\n";
			return nullptr;
		}
		auto resource = ObjectLibrary::getInstance().getResource(levelIdentifier);
		if (resource == nullptr)
		{
			std::cout << "Unable to find the " << levelType << " level with ID of '" << levelIdentifier << "'!\n";
			return nullptr;
		}

		return resource;
	}

	bool getLastModifiedTime(const std::string& filePath, std::time_t& time)
	{
		if (!boost::filesystem::exists(filePath))
		{
			return false;
		}

		time = boost::filesystem::last_write_time(filePath);
		return true;
	}

	void ShowExampleAppDockSpace(bool* p_open)
	{
		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_PassthruDockspace;
		bool opt_fullscreen = opt_fullscreen_persistant;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("RootDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}
		else
		{
			//ShowDockingDisabledMessage();
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
				if (ImGui::MenuItem("Open Scene")) {}
				if (ImGui::MenuItem("Save Scene")) {}
				if (ImGui::MenuItem("Save Scene As")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit", ""))
				{
					std::cout << "Exiting via the GUI is not currently supported.\n";
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				if (ImGui::MenuItem("Console")) {}
				if (ImGui::MenuItem("Asset Browser")) {}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();
	}

	int EditorWindow::Run(const std::string& projectPath, const std::string& levelIdentifier)
	{
		auto settings = std::make_unique<Settings::EngineSettings>(".\\engineSettings.yaml");

		// Load the project file
		ObjectLibrary::getInstance().initialize(settings->getEngineResourceDirectory(), projectPath);

		// Initialize the clock to this machine
		Timing::Clock::init();
		auto clock = std::make_unique<Timing::Clock>();

		auto display = std::make_shared<Rendering::Display>(settings->getWidth(), settings->getHeight(), "Derydoca Engine");
		display->init();

		// Divisor defines minimum frames per second
		unsigned long minFrameTime = 1000 / 60;

		auto editorScene = std::make_shared<Scenes::SerializedScene>();
		{
			auto editorSceneResource = getLevelResource(settings->getEditorComponentsSceneIdentifier(), "editor");
			editorScene->LoadFromFile(editorSceneResource->getSourceFilePath());
			editorScene->setUp();
		}

		Scenes::SceneManager& sceneManager = Scenes::SceneManager::getInstance();

		auto levelResource = getLevelResource(levelIdentifier, "scene");
		sceneManager.loadScene(levelResource);
		getLastModifiedTime(levelResource->getSourceFilePath(), m_levelLoadTime);

		// Initialize all components in the scene before rendering anything
		editorScene->getRoot()->init();
		sceneManager.getActiveScene()->getRoot()->init();

		// Run the post initialization routine on all components
		editorScene->getRoot()->postInit();
		sceneManager.getActiveScene()->getRoot()->postInit();

		std::vector<std::shared_ptr<Scenes::Scene>> scenes =
		{
			editorScene,
			sceneManager.getActiveScene()
		};

		std::time_t levelLastModifiedTime;

		// Render loop
		while (!display->isClosed())
		{

			// Tick the clock forward the number of ms it took since the last frame rendered
			editorScene->getRoot()->update(clock->getDeltaTime());
			sceneManager.getActiveScene()->getRoot()->update(clock->getDeltaTime());

			// Do any setup for a new frame
			display->newFrame();

			// Render all scene objects
			Rendering::CameraManager::getInstance().render(scenes);

			// Let the scene objects do whatever it is they need to do after rendering has completed this frame
			editorScene->getRoot()->postRender();
			sceneManager.getActiveScene()->getRoot()->postRender();

			// Render any editor GUIs that are defined on game components
			static bool dockSpaceVisible = true;
			ShowExampleAppDockSpace(&dockSpaceVisible);
			editorScene->getRoot()->renderEditorGUI();
			sceneManager.getActiveScene()->getRoot()->renderEditorGUI();

			// Let the display respond to any input events
			display->update();

			// Update the mouse inputs
			Input::InputManager::getInstance().getMouse()->update();

			// Pause the code execution if we are running faster than our capped frame rate
			unsigned long msToWait = (unsigned long)(minFrameTime - clock->getRenderTimeMS());
			if (msToWait > 0)
			{
				if (msToWait > minFrameTime)
				{
					msToWait = minFrameTime;
				}
				SDL_Delay(msToWait);
			}
			clock->update();

			// If the file has been updated since we last loaded it
			if (getLastModifiedTime(levelResource->getSourceFilePath(), levelLastModifiedTime) &&
				levelLastModifiedTime > m_levelLoadTime)
			{
				// Store the time that we are reloading the scene
				m_levelLoadTime = levelLastModifiedTime;

				// Reload the scene
				sceneManager.loadScene(levelResource);
				scenes =
				{
					editorScene,
					sceneManager.getActiveScene()
				};

				// Initialize the new scene
				sceneManager.getActiveScene()->getRoot()->init();
				sceneManager.getActiveScene()->getRoot()->postInit();
			}
		}

		// Clean up the scene
		editorScene->tearDown();
		sceneManager.getActiveScene()->tearDown();

		return 0;
	}

}
