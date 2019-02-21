#include "EditorPch.h"
#include "ResourcePicker.h"

bool DerydocaEngine::Dgui::ResourcePicker(const std::string label, const std::string& resourceName, const Resources::ResourceType resourceType)
{
	bool result = false;
	static bool opened = false;

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
			std::cout << "Opened" << std::endl;
		}

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
			std::cout << "Closed" << std::endl;
		}

	}

	// Return true if an resource was selected
	return result;
}
