#pragma once
#include "EditorPch.h"
#include "Rendering\Renderer.h"
#include "Rendering\Display.h"

namespace DerydocaEngine::Editor::Rendering
{

	class EditorRenderer : public DerydocaEngine::Rendering::RendererImplementation
	{
	public:
		EditorRenderer();
		~EditorRenderer();

		virtual void renderFrame();

	private:
		DerydocaEngine::Rendering::Display m_display;

	};

}
