#include "EnginePch.h"
#include "Renderer.h"
#include "Input\InputManager.h"
#include "Rendering\LightManager.h"
#include "GameObject.h"
#include "Rendering\MatrixStack.h"
#include "GraphicsAPI.h"

namespace DerydocaEngine::Rendering
{
	RendererImplementation::RendererImplementation(std::string title, int width, int height) :
		m_display(std::make_shared<Display>(width, height, title))
	{
	}

	void RendererImplementation::render(
		const glm::mat4& projectionMatrix,
		const std::shared_ptr<Scenes::Scene> scene
	)
	{
		// Extract the root element
		auto root = scene->getRoot();
		if (root == nullptr)
		{
			return;
		}
		
		glEnable(GL_DEPTH_TEST);

		// Clear the buffers
		GraphicsAPI::clearDepthBuffer();
		GraphicsAPI::clearColorBuffer({ 0.098f, 0.098f, 0.098f, 1 });

		// Run the pre-render methods in all components in the scene
		root->preRender();

		// Run the render methods in all components in the scene
		auto matrixStack = std::make_shared<Rendering::MatrixStack>();
		matrixStack->push(projectionMatrix);
		root->render(matrixStack);
		matrixStack->pop();
	}

	Renderer::Renderer(RendererImplementation& implementation) :
		m_minFrameTime(1000 / 60), // 60 FPS cap
		m_implementation(implementation)
	{
		Timing::Clock::init();
	}

	void Renderer::init()
	{
		m_implementation.init();
	}

	int Renderer::runRenderLoop()
	{
		while (!m_implementation.getDisplay()->isClosed())
		{
			// Have the renderer implementation render a frame
			m_implementation.renderFrame(m_clock.getDeltaTime());

			// Update the mouse inputs
			Input::InputManager::getInstance().getMouse()->update();

			// Let the display respond to any input events
			m_implementation.getDisplay()->update();

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
