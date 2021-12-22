#include "EditorWindow.h"
#include <random>

namespace DerydocaEngine::Editor::UI
{
	EditorWindow::EditorWindow()
		: m_Running(true)
		, m_Window()
	{
	}

	int EditorWindow::CreateWindowAndSwapChain()
	{
		SDL_Init(SDL_INIT_VIDEO);

		m_Window = SDL_CreateWindow(
			"Derydoca Engine",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			DEFAULT_WINDOW_WIDTH,
			DEFAULT_WINDOW_HEIGHT,
			SDL_WINDOW_OPENGL
		);

		if (m_Window == NULL)
		{
			return 1;
		}

		m_Renderer = SDL_CreateRenderer(
			m_Window,
			-1,
			SDL_RendererFlags::SDL_RENDERER_ACCELERATED | SDL_RendererFlags::SDL_RENDERER_PRESENTVSYNC | SDL_RendererFlags::SDL_RENDERER_TARGETTEXTURE
		);

		if (m_Renderer == NULL)
		{
			return 2;
		}

		return 0;
	}

	int EditorWindow::RunUpdateLoop()
	{
		while (m_Running)
		{
			ProcessWindowEvents();

			SDL_SetRenderDrawColor(m_Renderer, rand()%255, 149, 237, 255);
			SDL_RenderClear(m_Renderer);
			SDL_RenderPresent(m_Renderer);

			SDL_Delay(1000/60);
		}

		return 0;
	}

	void EditorWindow::Cleanup()
	{
		SDL_DestroyRenderer(m_Renderer);
		SDL_DestroyWindow(m_Window);

		SDL_Quit();
	}

	void EditorWindow::ProcessWindowEvents()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				m_Running = false;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					m_Running = false;
					break;
				}
				break;
			}
		}
	}
}
