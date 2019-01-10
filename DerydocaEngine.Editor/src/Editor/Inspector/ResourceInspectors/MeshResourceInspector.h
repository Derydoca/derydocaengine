#pragma once
#include "EditorPch.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Rendering\Mesh.h"
#include "Resources\MeshResource.h"

namespace DerydocaEngine::Editor::Inspector::ResourceInspectors
{

	class MeshResourceInspector : public InspectorRenderer
	{
		virtual void render(std::shared_ptr<Object> object)
		{
			auto resource = std::static_pointer_cast<Resources::MeshResource>(object);

			ImGui::Text("Mesh");
			ImGui::Text(resource->getName().c_str());

			auto mesh = std::static_pointer_cast<Rendering::Mesh>(resource->getResourceObjectPointer());

			ImGui::LabelText(std::to_string(mesh->getNumIndices()).c_str(), "Indices");
			ImGui::LabelText(std::to_string(mesh->getNumVertices()).c_str(), "Vertices");
		}
	};

}
