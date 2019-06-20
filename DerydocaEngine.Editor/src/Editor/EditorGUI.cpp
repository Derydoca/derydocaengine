#include "EditorPch.h"
#include "EditorGUI.h"
#include "Components\AnimationViewerWindow.h"
#include "Components\EditorCameraWindow.h"
#include "Components\EngineAssetBrowser.h"
#include "Components\EngineConsole.h"
#include "Editor\EditorRenderer.h"
#include "Rendering\Display.h"
#include "Rendering\DisplayManager.h"

DerydocaEngine::Editor::EditorGUI::EditorGUI()
{
}

DerydocaEngine::Editor::EditorGUI::~EditorGUI()
{
}

void DerydocaEngine::Editor::EditorGUI::render()
{
	static bool opt_fullscreen_persistant = true;
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_PassthruCentralNode;
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
	if (opt_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", (bool*)0, window_flags);
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
				auto display = Rendering::DisplayManager::getInstance().getDisplay(0);
				if (display != nullptr)
				{
					display->close();
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Console"))
			{
				EditorRenderer::GetInstance().addWindow(std::make_shared<Components::EngineConsole>());
			}
			if (ImGui::MenuItem("Asset Browser"))
			{
				EditorRenderer::GetInstance().addWindow(std::make_shared<Components::EngineAssetBrowser>());
			}
			if (ImGui::MenuItem("Editor Camera Window"))
			{
				EditorRenderer::GetInstance().addWindow(std::make_shared<Components::EditorCameraWindow>());
			}
			if (ImGui::MenuItem("Animation Viewer"))
			{
				EditorRenderer::GetInstance().addWindow(std::make_shared<Components::AnimationViewerWindow>());
			}
			ImGui::EndMenu();
		}

		bool isPlaying = EditorRenderer::GetInstance().isPlaying();
		if (!isPlaying && ImGui::MenuItem("Play"))
		{
			EditorRenderer::GetInstance().setPlaying(true);
		}
		if (isPlaying && ImGui::MenuItem("Pause"))
		{
			EditorRenderer::GetInstance().setPlaying(false);
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();
}
