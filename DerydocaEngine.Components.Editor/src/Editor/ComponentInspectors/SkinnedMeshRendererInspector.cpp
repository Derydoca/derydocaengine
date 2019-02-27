#include "EditorComponentsPch.h"
#include "SkinnedMeshRendererInspector.h"
#include "Dgui\ResourcePicker.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::SkinnedMeshRendererInspector::render(std::shared_ptr<Object> object)
{
	auto skinnedMeshRenderer = std::static_pointer_cast<Components::SkinnedMeshRenderer>(object);

	{
		std::shared_ptr<Resources::Resource> modifiedResource;
		if (Dgui::ResourcePicker("Animation", skinnedMeshRenderer->getAnimationResource(), Resources::AnimationResourceType, modifiedResource))
		{
			skinnedMeshRenderer->setAnimationResource(std::static_pointer_cast<Resources::AnimationResource>(modifiedResource));
		}
	}

	{
		std::shared_ptr<Resources::Resource> modifiedResource;
		if (Dgui::ResourcePicker("Mesh", skinnedMeshRenderer->getMeshResource(), Resources::MeshResourceType, modifiedResource))
		{
			skinnedMeshRenderer->setMeshResource(std::static_pointer_cast<Resources::MeshResource>(modifiedResource));
		}
	}

	{
		std::shared_ptr<Resources::Resource> modifiedResource;
		if (Dgui::ResourcePicker("Material", skinnedMeshRenderer->getMaterialResource(), Resources::MaterialResourceType, modifiedResource))
		{
			skinnedMeshRenderer->setMaterialResource(std::static_pointer_cast<Resources::MaterialResource>(modifiedResource));
		}
	}

}
