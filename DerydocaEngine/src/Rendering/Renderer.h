#pragma once

namespace DerydocaEngine::Rendering
{

	class RendererImplementation
	{
		virtual void renderFrame() = 0;
	};

	class Renderer
	{
	public:
		Renderer(std::shared_ptr<RendererImplementation> implementation);

		void loadProject(boost::filesystem::path path);

		int runRenderLoop();

	private:
		std::shared_ptr<RendererImplementation> m_implementation;

	};

}