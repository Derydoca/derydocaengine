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
		std::shared_ptr<DerydocaEngine::Rendering::Display> getDisplay() { return m_display; }
	protected:
		std::shared_ptr<DerydocaEngine::Rendering::Display> m_display;
	};

	class Renderer
	{
	public:
		Renderer(RendererImplementation& implementation);

		void init();
		int runRenderLoop();

	private:
		unsigned long m_minFrameTime;
		Timing::Clock m_clock;
		RendererImplementation& m_implementation;

	};

}