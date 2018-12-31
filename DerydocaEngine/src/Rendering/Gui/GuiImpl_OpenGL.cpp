#include "EnginePch.h"
#include "GuiImpl.h"

#include <vendor/imgui/imgui_impl_sdl.h>
#include <vendor/imgui/imgui_impl_opengl3.h>

namespace DerydocaEngine::Rendering::Gui
{

	ImGuiIO GuiImpl::init(SDL_Window* window, SDL_GLContext& context)
	{
		char* GLSL_VERSION = "#version 130";
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		auto guiIO = ImGui::GetIO(); (void)guiIO;

		ImGui::StyleColorsDark();
		
		ImGui_ImplSDL2_InitForOpenGL(window, context);
		ImGui_ImplOpenGL3_Init(GLSL_VERSION);
		return guiIO;
	}

	void GuiImpl::newFrame(SDL_Window* window)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		static bool show_demo_w = true;
		if (show_demo_w)
		{
			ImGui::ShowDemoWindow(&show_demo_w);
		}
	}

	void GuiImpl::render(SDL_Window* window, SDL_GLContext& context)
	{
		ImGui::Render();
		SDL_GL_MakeCurrent(window, context);
		glViewport(0, 0, 300, 300);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GuiImpl::shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

}