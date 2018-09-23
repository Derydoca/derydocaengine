#pragma once
#include <string>
#include "Camera.h"

typedef void *SDL_GLContext;
struct SDL_Window;
namespace DerydocaEngine::Input {
	class Keyboard;
}

namespace DerydocaEngine::Rendering
{

	class Display
	{
	public:
		Display(int const& width, int const& height, std::string const& title);
		virtual ~Display();

		void init(int const& width, int const& height, std::string const& title);
		void update();
		bool isClosed();
		inline float getAspectRatio() { return (float)m_width / (float)m_height; }

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }
		inline SDL_Window* getWindow() const { return m_window; }

		void bindAsRenderTarget();
		void registerCamera(Components::Camera* const& cam) { m_camera = cam; }
		void unregisterCamera() { m_camera = nullptr; }
	private:
		Display(Display const& other) {}
		Display& operator=(Display const& other) {}

		void windowSizeChanged(int const& width, int const& height);

		SDL_Window* m_window;
		SDL_GLContext m_glContext;
		bool m_isClosed;
		int m_width;
		int m_height;
		Input::Keyboard* m_keyboard;
		Components::Camera* m_camera;
	};

}
