#include "EnginePch.h"
#include "Renderer.h"

DerydocaEngine::Rendering::Renderer::Renderer(std::shared_ptr<RendererImplementation> implementation) :
	m_implementation(implementation)
{
}

void DerydocaEngine::Rendering::Renderer::loadProject(boost::filesystem::path path)
{
}

int DerydocaEngine::Rendering::Renderer::runRenderLoop()
{
	return 0;
}
