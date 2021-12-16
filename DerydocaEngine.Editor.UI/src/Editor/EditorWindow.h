#include <memory>
#include <SDL2/SDL.h>

//template <class T> using sptr = std::shared_ptr<T>;
//template <class T> using uptr = std::unique_ptr<T>;

namespace DerydocaEngine::Editor::UI
{
	class EditorWindow
	{
	public:
		const int DEFAULT_WINDOW_HEIGHT = 600;
		const int DEFAULT_WINDOW_WIDTH = 800;

	public:
		EditorWindow();

		int CreateWindowAndSwapChain();
		int RunUpdateLoop();
		void Cleanup();

	private:
		void ProcessWindowEvents();

	private:
		bool m_Running;
		SDL_Window* m_Window;
		SDL_Renderer* m_Renderer;

	};
}