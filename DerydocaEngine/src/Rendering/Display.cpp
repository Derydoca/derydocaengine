#include "EnginePch.h"
#include "Rendering\Display.h"

#include <GL/glew.h>
#include "Components\Camera.h"
#include "Input\InputManager.h"
#include "Rendering\DisplayManager.h"
#include "Rendering\Gui\GuiImpl.h"

#include <sdl2/SDL.h>
#undef main

namespace DerydocaEngine::Rendering
{
	Display::Display(int const& width, int const& height, std::string const& title)
	{
		DisplayManager::getInstance().addDisplay(this);

		m_width = width;
		m_height = height;

		// TODO: Look into reducing the number of items included
		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 3);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 3);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 2);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_DisplayMode current;
		SDL_GetCurrentDisplayMode(0, &current);
		m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		m_glContext = SDL_GL_CreateContext(m_window);
		SDL_GL_SetSwapInterval(1); // VSync

		// Enable multisampling
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

		GLenum status = glewInit();

		if (status != GLEW_OK) {
			std::cerr << "Unable to initialize OpenGL loader!\n";
		}

		m_isClosed = false;

		glEnable(GL_DEPTH_TEST);

		// TODO: Reenable after working through the opengl shader cookbook and evaluate a better way of handling this on a per-mesh basis
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		// Print information about the current instance of OpenGL
		const GLubyte *renderer = glGetString(GL_RENDERER);
		const GLubyte *vendor = glGetString(GL_VENDOR);
		const GLubyte *version = glGetString(GL_VERSION);
		const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
		std::cout << "Renderer: " << renderer << "\n";
		std::cout << "Vendor: " << vendor << "\n";
		std::cout << "Version: " << version << "\n";
		std::cout << "GLSL Version: " << glslVersion << "\n";
		std::cout << "Max Vertex Attributes: " << GL_MAX_VERTEX_ATTRIBS << "\n";

		/*
		GLint nExtensions;
		glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
		std::cout << "Supported extensions (" << nExtensions << "):" << "\n";
		for (GLint i = 0; i < nExtensions; i++)
		{
			std::cout << "    " << i << ": " << glGetStringi(GL_EXTENSIONS, i) << "\n";
		}
		*/

		m_keyboard = Input::InputManager::getInstance().getKeyboard();
	}

	Display::~Display()
	{
		Gui::GuiImpl::shutdown();

		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

	bool Display::isClosed() {
		return m_isClosed;
	}

	void Display::bindAsRenderTarget()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void Display::windowSizeChanged(int const& width, int const& height)
	{
		m_width = width;
		m_height = height;

		if (m_camera)
		{
			m_camera->resize(width, height);
		}
	}

	void Display::init()
	{
		Gui::GuiImpl::init(m_window, m_glContext);
	}

	void Display::newFrame() {
		Gui::GuiImpl::newFrame(m_window);
	}

	void Display::update() {
		Gui::GuiImpl::render(m_window, m_glContext);
		SDL_GL_SwapWindow(m_window);

		m_keyboard->update();

		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);

			switch (e.type) {
			case SDL_QUIT:
				m_isClosed = true;
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					m_isClosed = true;
					m_keyboard->setKeyState(e.key.keysym.sym, true);
					break;
				default:
					m_keyboard->setKeyState(e.key.keysym.sym, true);
					break;
				}
				break;
			case SDL_KEYUP:

				m_keyboard->setKeyState(e.key.keysym.sym, false);
				break;
			case SDL_WINDOWEVENT:
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					windowSizeChanged(e.window.data1, e.window.data2);

					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
	}

}
