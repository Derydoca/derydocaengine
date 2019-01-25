#pragma once
#include <string>

typedef void *SDL_GLContext;
struct SDL_Window;
namespace DerydocaEngine {
	namespace Components {
		class Camera;
	}
	namespace Input {
		class Keyboard;
	}
}

namespace DerydocaEngine::Rendering
{

	class Display
	{
	public:
		Display(int const& width, int const& height, std::string const& title);
		virtual ~Display();

		void init();
		void newFrame();
		void update();
		bool isClosed();
		inline float getAspectRatio() { return (float)m_width / (float)m_height; }

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }
		inline SystemWindow* getWindow() const { return m_window; }
		inline GraphicsAPIContext* getContext() { return &(m_context); }

		void setSize(int width, int height);

		void bindAsRenderTarget();
		void registerCamera(Components::Camera* const& cam) { m_camera = cam; }
		void unregisterCamera() { m_camera = nullptr; }
	private:
		Display(Display const& other) {}
		Display& operator=(Display const& other) {}

		void windowSizeChanged(int const& width, int const& height);

		SystemWindow* m_window;
		GraphicsAPIContext m_context;
		bool m_isClosed;
		int m_width;
		int m_height;
		Input::Keyboard* m_keyboard;
		Components::Camera* m_camera;
	};

}
