#pragma once
#include <string>
#include "Settings/EditorWindowSettings.h"

typedef void *SDL_GLContext;
struct SDL_Window;
namespace DerydocaEngine {
	namespace Components {
		class Camera;
	}
	namespace Input {
		class Keyboard;
		class Mouse;
	}
}

namespace DerydocaEngine::Rendering
{

	class Display
	{
	public:
		Display(int const& width, int const& height, std::string const& title);
		virtual ~Display();

		void close() { m_isClosed = true; }
		void init();
		void newFrame();
		void update();
		bool isClosed();
		inline float getAspectRatio() { return (float)m_actualSize.x / (float)m_actualSize.y; }

		inline int2 getNonMaximizedSize() const {
			return m_windowState == Settings::WindowState::Normal ? m_actualSize : m_lastSize;
		}
		inline int2 getActualSize() const { return m_actualSize; }
		inline const Settings::WindowState getWindowState() const { return m_windowState; }
		inline SystemWindow* getWindow() const { return m_window; }
		inline GraphicsAPIContext* getContext() { return &(m_context); }

		void setSize(int2 size);
		void setFullScreen(bool isFullScreen);

		void bindAsRenderTarget();
		void maximize();
		void registerCamera(Components::Camera* const& cam) { m_camera = cam; }
		void unregisterCamera() { m_camera = nullptr; }
	private:
		Display(Display const& other) {}
		Display& operator=(Display const& other) {}

		void windowSizeChanged(int const& width, int const& height);

		SystemWindow* m_window;
		GraphicsAPIContext m_context;
		bool m_hasLoadedInitialDimensions;
		bool m_isClosed;
		Settings::WindowState m_windowState;
		int2 m_lastSize;
		int2 m_actualSize;
		int2 m_windowSize;
		Input::Keyboard* m_keyboard;
		Input::Mouse* m_mouse;
		Components::Camera* m_camera;
	};

}
