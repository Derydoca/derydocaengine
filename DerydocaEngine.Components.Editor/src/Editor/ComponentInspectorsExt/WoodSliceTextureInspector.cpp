#include "EditorComponentsPch.h"
#include "WoodSliceTextureInspector.h"
#include "Editor\Inspector\InspectorRendererFactory.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::WoodSliceTextureInspector::render(std::shared_ptr<Object> object)
{
	auto wood = std::static_pointer_cast<DerydocaEngine::Ext::WoodSliceTexture>(object);

	InspectorRendererFactory::getInstance().renderInspector(wood->getSliceMatrixTransform());
}
