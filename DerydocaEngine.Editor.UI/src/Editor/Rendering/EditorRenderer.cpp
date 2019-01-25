#include "EditorPch.h"
#include "EditorRenderer.h"
#include "Settings\EngineSettings.h"
#include "Rendering\CameraManager.h"
#include "Rendering\Gui\DearImgui.h"
#include "Scenes\SceneManager.h"

namespace DerydocaEngine::Editor::Rendering
{

	EditorRenderer::EditorRenderer() :
		RendererImplementation("Derydoca Engine - Editor", 300, 300),
		m_editorComponentsScene(std::make_shared<Scenes::SerializedScene>()),
		m_editorGuiScene(std::make_shared<Scenes::SerializedScene>())
	{
		DerydocaEngine::Rendering::Gui::DearImgui::init(m_display);
	}

	EditorRenderer::~EditorRenderer()
	{
		DerydocaEngine::Rendering::Gui::DearImgui::shutdown();
	}

	void EditorRenderer::init()
	{
		// Load the settings
		auto settings = DerydocaEngine::Settings::EngineSettings(".\\engineSettings.yaml");
		m_display->setSize(settings.getWidth(), settings.getHeight());
		m_display->init();

		// Load the scenes
		loadScene(settings.getEditorComponentsSceneIdentifier(), m_editorComponentsScene);
		loadScene(settings.getEditorGuiSceneIdentifier(), m_editorGuiScene);
	}

	void ShowExampleAppDockSpace()
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
		bool isOpen = true;
		ImGui::Begin("DockSpace Demo", &isOpen, window_flags);
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

	void EditorRenderer::renderFrame(const float deltaTime)
	{
		auto scene = Scenes::SceneManager::getInstance().getActiveScene();

		// Initialize a new immediate mode GUI frame
		DerydocaEngine::Rendering::Gui::DearImgui::newFrame(m_display);

		// Update
		m_editorComponentsScene->getRoot()->update(deltaTime);
		m_editorGuiScene->getRoot()->update(deltaTime);
		scene->getRoot()->update(deltaTime);

		// ImGui new frame

		// Render
		DerydocaEngine::Rendering::CameraManager::getInstance().render({
			m_editorComponentsScene,
			m_editorGuiScene,
			scene
			});

		// Post render
		m_editorComponentsScene->getRoot()->postRender();
		m_editorGuiScene->getRoot()->postRender();
		scene->getRoot()->postRender();

		// Render editor GUI
		ShowExampleAppDockSpace();
		m_editorGuiScene->getRoot()->renderEditorGUI();

		// Render the immediate mode GUI frame to the framebuffer
		DerydocaEngine::Rendering::Gui::DearImgui::render(m_display);
	}

	std::shared_ptr<Resources::LevelResource> EditorRenderer::getSceneResource(const std::string& sceneId, const std::string& sceneType)
	{
		// Validate that the sceneId parameter is populated
		if (sceneId.empty())
		{
			std::cout << "No identifier was provided for the " << sceneType << " scene!\n";
			return nullptr;
		}

		// Get the scene resource object with the ID of sceneId
		auto resource = ObjectLibrary::getInstance().getResource<Resources::LevelResource>(sceneId);
		if (resource == nullptr)
		{
			std::cout << "Unable to find the " << sceneType << " scene with ID of '" << sceneId << "'!\n";
			return nullptr;
		}

		// Return the found resource
		return resource;
	}

	void EditorRenderer::loadScene(const std::string& sceneId, std::shared_ptr<Scenes::SerializedScene> scene)
	{
		auto editorComponentsSceneResource = getSceneResource(sceneId, "editor components");
		if (editorComponentsSceneResource != nullptr)
		{
			scene->LoadFromFile(editorComponentsSceneResource->getSourceFilePath());
			scene->setUp();
			scene->getRoot()->init();
			scene->getRoot()->postInit();
		}
	}

}
