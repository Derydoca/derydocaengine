#pragma once
#include "EnginePch.h"

namespace DerydocaEngine
{

	class SystemWindowingLayer
	{
	public:
		static void init();
		static SystemWindow* createWindow(std::string title, int width, int height);
		static GraphicsAPIContext createGraphicsAPIContext(SystemWindow* window);
		static bool setVSync(bool enabled);
		static void swapBuffers(SystemWindow* window, GraphicsAPIContext* graphicsApiContext);
		static bool setMultisampling(int buffers, int samples);
		static void setDepthTesting(bool enabled);
	};

}
