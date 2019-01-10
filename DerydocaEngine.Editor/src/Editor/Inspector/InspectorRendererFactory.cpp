#include "EditorPch.h"
#include "InspectorRendererFactory.h"
#include "Editor\Inspector\ResourceInspectors\AnimationResourceInspector.h"
#include "Editor\Inspector\ResourceInspectors\CubemapResourceInspector.h"
#include "Editor\Inspector\ResourceInspectors\FontResourceInspector.h"
#include "Editor\Inspector\ResourceInspectors\LevelResourceInspector.h"
#include "Editor\Inspector\ResourceInspectors\MaterialResourceInspector.h"
#include "Editor\Inspector\ResourceInspectors\MeshResourceInspector.h"
#include "Editor\Inspector\ResourceInspectors\RasterFontInspector.h"
#include "Editor\Inspector\ResourceInspectors\ShaderResourceInspector.h"
#include "Editor\Inspector\ResourceInspectors\TextureResourceInspector.h"
#include "Editor\Inspector\UndefinedInspector.h"

namespace DerydocaEngine::Editor::Inspector
{
	
	InspectorRendererFactory::InspectorRendererFactory() :
		m_renderFunctions()
	{
		registerUndefinedRenderer(std::make_shared<UndefinedInspector>());
		registerRenderer<Resources::AnimationResource>(std::make_shared<ResourceInspectors::AnimationResourceInspector>());
		registerRenderer<Resources::CubemapResource>(std::make_shared<ResourceInspectors::CubemapResourceInspector>());
		registerRenderer<Resources::FontResource>(std::make_shared<ResourceInspectors::FontResourceInspector>());
		registerRenderer<Resources::LevelResource>(std::make_shared<ResourceInspectors::LevelResourceInspector>());
		registerRenderer<Resources::MaterialResource>(std::make_shared<ResourceInspectors::MaterialResourceInspector>());
		registerRenderer<Resources::MeshResource>(std::make_shared<ResourceInspectors::MeshResourceInspector>());
		registerRenderer<Resources::RasterFontResource>(std::make_shared<ResourceInspectors::RasterFontResourceInspector>());
		registerRenderer<Resources::ShaderResource>(std::make_shared<ResourceInspectors::ShaderResourceInspector>());
		registerRenderer<Resources::TextureResource>(std::make_shared<ResourceInspectors::TextureResourceInspector>());
	}

	InspectorRendererFactory::~InspectorRendererFactory()
	{
	}

}
