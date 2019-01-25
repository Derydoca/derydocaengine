#include "EnginePch.h"
#include "DearImgui.h"

#include <vendor/imgui/imgui_impl_sdl.h>
#if (OPENGL == 1)
#include <vendor/imgui/imgui_impl_opengl3.h>
#endif

namespace DerydocaEngine::Rendering::Gui
{

	ImGuiIO DearImgui::init(std::shared_ptr<Display> display)
	{
		char* GLSL_VERSION = "#version 130";
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigDockingWithShift = true;
		io.ConfigWindowsResizeFromEdges = true;
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplSDL2_InitForOpenGL(display->getWindow(), display->getContext());
#if (OPENGL == 1)
		ImGui_ImplOpenGL3_Init(GLSL_VERSION);
#endif
		return io;
	}

	void DearImgui::newFrame(std::shared_ptr<Display> display)
	{
#if (OPENGL == 1)
		ImGui_ImplOpenGL3_NewFrame();
#endif
		ImGui_ImplSDL2_NewFrame(display->getWindow());
		ImGui::NewFrame();
	}

	void DearImgui::render(std::shared_ptr<Display> display)
	{
		ImGui::Render();
		//SDL_GL_MakeCurrent(display->getWindow(), display->getContext());
#if (OPENGL == 1)
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
	}

	void DearImgui::shutdown()
	{
#if (OPENGL == 1)
		ImGui_ImplOpenGL3_Shutdown();
#endif
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	bool DearImgui::processEvent(SDL_Event& event)
	{
		return ImGui_ImplSDL2_ProcessEvent(&event);
	}

}