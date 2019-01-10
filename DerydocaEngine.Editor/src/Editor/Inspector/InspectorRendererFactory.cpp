#include "EditorPch.h"
#include "InspectorRendererFactory.h"
#include "Editor\Inspector\UndefinedInspector.h"
#include "Editor\Inspector\LevelResourceInspector.h"

namespace DerydocaEngine::Editor::Inspector
{
	
	InspectorRendererFactory::InspectorRendererFactory() :
		m_renderFunctions()
	{
		registerUndefinedRenderer(std::make_shared<UndefinedInspector>());
		registerRenderer<Resources::LevelResource>(std::make_shared<LevelResourceInspector>());
	}

	InspectorRendererFactory::~InspectorRendererFactory()
	{
	}

}
