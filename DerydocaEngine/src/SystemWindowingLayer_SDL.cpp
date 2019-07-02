#include "EnginePch.h"

#if OPENGL||VULKAN

#include "SystemWindowingLayer.h"

namespace DerydocaEngine
{

	void SystemWindowingLayer::init()
	{
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
	}

	SystemWindow* SystemWindowingLayer::createWindow(std::string title, int width, int height)
	{
		Uint32 windowFlags = SDL_WINDOW_RESIZABLE;
#if OPENGL
		windowFlags |= SDL_WINDOW_OPENGL;
#elif VULKAN
		windowFlags |= SDL_WINDOW_VULKAN;
#endif
		return SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
	}

	GraphicsAPIContext SystemWindowingLayer::createGraphicsAPIContext(SystemWindow* window)
	{
#if OPENGL
		return SDL_GL_CreateContext(window);
#else
		D_LOG_CRITICAL("Unable to create a context for the current rendering API.");
		return nullptr;
#endif
	}

	bool SystemWindowingLayer::setVSync(bool enabled)
	{
		return SDL_GL_SetSwapInterval(enabled ? 1 : 0) == 0;
	}

	void SystemWindowingLayer::swapBuffers(SystemWindow * window, GraphicsAPIContext * graphicsApiContext)
	{
		SDL_GL_SwapWindow(window);
	}

	bool SystemWindowingLayer::setMultisampling(int buffers, int samples)
	{
		int result = 0;
		result += SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, buffers);
		result += SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, samples);
		return result == 0;
	}

	void SystemWindowingLayer::setDepthTesting(bool enabled)
	{
		if (enabled)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

}

#endif
