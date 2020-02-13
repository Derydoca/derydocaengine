#include "EnginePch.h"
#include "Rendering\Display.h"

#include "Components\Camera.h"
#include "Input\InputManager.h"
#include "Rendering\DisplayManager.h"
#include "Rendering\GraphicsAPI.h"
#include "Rendering\Gui\DearImgui.h"
#include "SystemWindowingLayer.h"

namespace DerydocaEngine::Rendering
{
	Display::Display(int const& width, int const& height, std::string const& title) :
		m_window(0),
		m_context(),
		m_hasLoadedInitialDimensions(false),
		m_isClosed(false),
		m_windowState(Settings::WindowState::Normal),
		m_lastSize(width, height),
		m_actualSize(width, height),
		m_windowSize(width, height),
		m_keyboard(0),
		m_mouse(0),
		m_camera(0)
	{
		DisplayManager::getInstance().addDisplay(this);

		m_keyboard = Input::InputManager::getInstance().getKeyboard();
		m_mouse = Input::InputManager::getInstance().getMouse();

		m_window = SystemWindowingLayer::createWindow(title, m_actualSize.x, m_actualSize.y);
		m_context = SystemWindowingLayer::createGraphicsAPIContext(m_window);
		
		static bool graphicsAPIInitialized = false;
		if (!graphicsAPIInitialized)
		{
			GraphicsAPI::init();
			graphicsAPIInitialized = true;
		}

		// Clear the screen so it is filled with black
		SystemWindowingLayer::swapBuffers(m_window, &m_context);
	}

	Display::~Display()
	{
		//Gui::DearImgui::shutdown();

		SDL_GL_DeleteContext(m_context);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

	bool Display::isClosed() {
		return m_isClosed;
	}

	void Display::setSize(int2 size)
	{
		m_actualSize = size;
		SDL_SetWindowSize(m_window, m_actualSize.x, m_actualSize.y);

		// Clear the screen to black
		SystemWindowingLayer::swapBuffers(m_window, &m_context);
	}

	void Display::setFullScreen(bool isFullScreen)
	{
		m_windowState = Settings::WindowState::FullScreen;
		SDL_SetWindowFullscreen(m_window, isFullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	}

	void Display::maximize()
	{
		m_windowState = Settings::WindowState::Maximized;
		SDL_MaximizeWindow(m_window);
	}

	void Display::bindAsRenderTarget()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void Display::windowSizeChanged(int const& width, int const& height)
	{
		m_lastSize = m_actualSize;
		m_actualSize = { width, height };

		if (m_windowState == Settings::WindowState::Normal && m_hasLoadedInitialDimensions)
		{
			m_windowSize = { width, height };
		}

		if (m_camera)
		{
			m_camera->resize(width, height);
		}

		if (!m_hasLoadedInitialDimensions)
		{
			m_hasLoadedInitialDimensions = true;
		}
	}

	void Display::init()
	{
		//Gui::DearImgui::init(m_window, m_context);
	}

	void Display::newFrame() {
		//Gui::DearImgui::newFrame(m_window);
	}

	void Display::update() {
		//Gui::DearImgui::render(m_window, m_context);
		SystemWindowingLayer::swapBuffers(m_window, &m_context);

		m_keyboard->update();

		SDL_Event e;

		while (SDL_PollEvent(&e)) {
			// TODO: Move this someplace else when input layers are implemented
			Gui::DearImgui::processEvent(e);

			switch (e.type) {
			case SDL_QUIT:
				m_isClosed = true;
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEWHEEL:
				m_mouse->setRelativeWheelMovement(e.wheel.x, e.wheel.y);
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
				case SDL_WINDOWEVENT_MAXIMIZED:
					m_windowState == Settings::WindowState::Maximized;
					
					break;
				case SDL_WINDOWEVENT_RESTORED:
					m_windowState == Settings::WindowState::Normal;

					break;
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
