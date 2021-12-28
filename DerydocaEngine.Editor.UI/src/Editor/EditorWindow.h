#include <memory>
#include <GLFW/glfw3.h>

namespace DerydocaEngine::Editor::UI
{
	struct EditorWindowSettings
	{
		uint32_t BackbufferWidth = 1280;
		uint32_t BackbufferHeight = 720;
		uint32_t SwapChainSampleCount = 1;
		uint32_t RefreshRate = 0;
		int WindowPosX = -1;
		int WindowPosY = -1;
	};

	class EditorWindow
	{
	public:
		const int DEFAULT_WINDOW_HEIGHT = 600;
		const int DEFAULT_WINDOW_WIDTH = 800;

	public:
		EditorWindow();

		int CreateWindowAndSwapChain(EditorWindowSettings settings, const char* windowTitle);
		int RunUpdateLoop();
		void Cleanup();

	private:
		void ProcessWindowEvents();

	private:
		bool m_Running;
		GLFWwindow* m_Window;
		EditorWindowSettings m_WindowParams;
		const char* m_WindowTitle;

	};
}