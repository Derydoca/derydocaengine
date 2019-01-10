#include "EditorPch.h"
#include "InspectorRendererFactory.h"
#include "Editor\Inspector\AnimationResourceInspector.h"
#include "Editor\Inspector\CubemapResourceInspector.h"
#include "Editor\Inspector\FontResourceInspector.h"
#include "Editor\Inspector\LevelResourceInspector.h"
#include "Editor\Inspector\TextureResourceInspector.h"
#include "Editor\Inspector\UndefinedInspector.h"
#include "Editor\Inspector\MaterialInspector.h"

namespace DerydocaEngine::Editor::Inspector
{
	
	InspectorRendererFactory::InspectorRendererFactory() :
		m_renderFunctions()
	{
		registerUndefinedRenderer(std::make_shared<UndefinedInspector>());
		registerRenderer<Resources::AnimationResource>(std::make_shared<AnimationResourceInspector>());
		registerRenderer<Resources::CubemapResource>(std::make_shared<CubemapResourceInspector>());
		registerRenderer<Resources::FontResource>(std::make_shared<FontResourceInspector>());
		registerRenderer<Resources::LevelResource>(std::make_shared<LevelResourceInspector>());
		registerRenderer<Resources::MaterialResource>(std::make_shared<MaterialResourceInspector>());
		registerRenderer<Resources::TextureResource>(std::make_shared<TextureResourceInspector>());
	}

	InspectorRendererFactory::~InspectorRendererFactory()
	{
	}

}
