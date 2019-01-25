#include "EditorPch.h"
#include "EditorRenderer.h"
#include "Settings\EngineSettings.h"

DerydocaEngine::Editor::Rendering::EditorRenderer::EditorRenderer() :
	m_display(300, 300, "Derydoca Engine - Editor")
{
	// Load the settings
	auto settings = DerydocaEngine::Settings::EngineSettings(".\\engineSettings.yaml");
	m_display.setSize(settings.getWidth(), settings.getHeight());
	m_display.init();
}

DerydocaEngine::Editor::Rendering::EditorRenderer::~EditorRenderer()
{
}

void DerydocaEngine::Editor::Rendering::EditorRenderer::renderFrame()
{
}
