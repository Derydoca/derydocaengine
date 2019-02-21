#include "EditorPch.h"
#include "ResourcePicker.h"
#include "ObjectLibrary.h"

bool DerydocaEngine::Dgui::ResourcePicker(const std::string label, const std::string& resourceName, const Resources::ResourceType resourceType)
{
	bool result = false;
	static bool opened = false;
	static std::vector<std::shared_ptr<Resources::Resource>> resources;

	// Button to open asset picker
	if (ImGui::Button(resourceName.c_str()))
	{
		ImGui::OpenPopup("Resource Picker");
	}

	// Field name
	ImGui::SameLine();
	ImGui::Text(label.c_str());

	// Modal popup
	if (ImGui::BeginPopupModal("Resource Picker", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (!opened)
		{
			// Initialize the list
			opened = true;
			resources = ObjectLibrary::getInstance().getResourcesOfType(resourceType);
		}

		ImGui::BeginChild("Resource List", ImVec2(ImGui::GetWindowContentRegionWidth(), 500), true, ImGuiWindowFlags_None);
		for (auto resource : resources)
		{
			ImGui::Text(resource->getName().c_str());
		}
		ImGui::EndChild();

		ImGui::TextWrapped("Select your resource.");
		if (ImGui::Button("Select"))
		{
			result = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	else
	{
		if (opened)
		{
			opened = false;
			resources.clear();
		}

	}

	// Return true if an resource was selected
	return result;
}
