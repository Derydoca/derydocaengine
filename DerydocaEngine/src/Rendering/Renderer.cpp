#include "EnginePch.h"
#include "Renderer.h"
#include "Input\InputManager.h"

namespace DerydocaEngine::Rendering
{
	RendererImplementation::RendererImplementation(std::string title, int width, int height) :
		m_display(std::make_shared<Display>(width, height, title))
	{
	}

	Renderer::Renderer(std::shared_ptr<RendererImplementation> implementation) :
		m_minFrameTime(1000 / 60), // 60 FPS cap
		m_implementation(implementation)
	{
		Timing::Clock::init();
	}

	void Renderer::init()
	{
		m_implementation->init();
	}

	int Renderer::runRenderLoop()
	{
		while (!m_implementation->getDisplay()->isClosed())
		{
			// Have the renderer implementation render a frame
			m_implementation->renderFrame(m_clock.getDeltaTime());

			// Let the display respond to any input events
			m_implementation->getDisplay()->update();

			// Update the mouse inputs
			Input::InputManager::getInstance().getMouse()->update();

			// If we have rendered our frame under the FPS limit, wait until it's time to render the next frame
			unsigned long msToWait = (unsigned long)(m_minFrameTime - m_clock.getRenderTimeMS());
			if (msToWait > 0)
			{
				SDL_Delay(msToWait);
			}
			m_clock.update();
		}
		return 0;
	}

}
