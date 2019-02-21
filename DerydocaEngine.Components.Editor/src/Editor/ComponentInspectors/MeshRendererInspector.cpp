#include "EditorComponentsPch.h"
#include "MeshRendererInspector.h"
#include "Rendering\Material.h"
#include "Rendering\Mesh.h"
#include "Dgui\ResourcePicker.h"

void DerydocaEngine::Editor::Inspector::ComponentInspectors::MeshRendererInspector::render(std::shared_ptr<Object> object)
{
	auto meshRenderer = std::static_pointer_cast<Components::MeshRenderer>(object);

	ImGui::Text("A more comprehensive inspector will come in the future.");

	auto material = meshRenderer->getMaterial();
	{
		auto resource = Dgui::ResourcePicker("Material", "my_material", Resources::ResourceType::MaterialResourceType);
		if (resource)
		{
			meshRenderer->setMaterial(std::static_pointer_cast<Rendering::Material>(resource->getResourceObjectPointer()));
		}
	}

	{
		auto mesh = meshRenderer->getMesh();
		auto resource = Dgui::ResourcePicker("Mesh", "my_mesh", Resources::ResourceType::MeshResourceType);
		if (resource)
		{
			meshRenderer->setMesh(std::static_pointer_cast<Rendering::Mesh>(resource->getResourceObjectPointer()));
		}
	}

}
