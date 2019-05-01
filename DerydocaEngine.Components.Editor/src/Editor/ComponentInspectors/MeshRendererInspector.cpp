#include "EditorComponentsPch.h"
#include "MeshRendererInspector.h"
#include "Rendering\Material.h"
#include "Rendering\Mesh.h"
#include "Dgui\ResourcePicker.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::MeshRendererInspector::render(std::shared_ptr<Object> object)
{
	auto meshRenderer = std::static_pointer_cast<Components::MeshRenderer>(object);

	{
		std::shared_ptr<Resources::Resource> modifiedResource;
		auto resource = Dgui::ResourcePicker("Material", meshRenderer->getMaterialResource(), Resources::ResourceType::MaterialResourceType, modifiedResource);
		if (resource)
		{
			meshRenderer->setMaterial(std::static_pointer_cast<Resources::MaterialResource>(modifiedResource));
		}
	}

	{
		std::shared_ptr<Resources::Resource> modifiedResource;
		if (Dgui::ResourcePicker("Mesh", meshRenderer->getMeshResource(), Resources::ResourceType::MeshResourceType, modifiedResource))
		{
			meshRenderer->setMesh(std::static_pointer_cast<Resources::MeshResource>(modifiedResource));
		}
	}

}
