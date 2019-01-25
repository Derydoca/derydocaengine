#pragma once
#include "Rendering\Display.h"
#include "Timing\Clock.h"

namespace DerydocaEngine::Rendering
{

	class RendererImplementation
	{
	public:
		RendererImplementation(std::string title, int width, int height);

		virtual void init() = 0;
		virtual void renderFrame(const float deltaTime) = 0;
		DerydocaEngine::Rendering::Display& getDisplay() { return m_display; }
	protected:
		DerydocaEngine::Rendering::Display m_display;
	};

	class Renderer
	{
	public:
		Renderer(std::shared_ptr<RendererImplementation> implementation);

		void init();
		int runRenderLoop();

	private:
		unsigned long m_minFrameTime;
		Timing::Clock m_clock;
		std::shared_ptr<RendererImplementation> m_implementation;

	};

}