#include "EnginePch.h"
#include "GuiImpl.h"

#include <vendor/imgui/imgui_impl_sdl.h>
#include <vendor/imgui/imgui_impl_opengl3.h>

namespace DerydocaEngine::Rendering::Gui
{

	ImGuiIO GuiImpl::init(SDL_Window* window, SDL_GLContext* context)
	{
		char* GLSL_VERSION = "#version 150";
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		auto guiIO = ImGui::GetIO(); (void)guiIO;
		ImGui_ImplSDL2_InitForOpenGL(window, context);
		ImGui_ImplOpenGL3_Init(GLSL_VERSION);
		ImGui::StyleColorsDark();
		return guiIO;
	}

	void GuiImpl::newFrame(SDL_Window* window)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
	}

	void GuiImpl::render()
	{
		ImGui::Render();
	}

	void GuiImpl::shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

}