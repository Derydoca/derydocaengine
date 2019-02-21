#include "EditorPch.h"
#include "ResourcePicker.h"
#include "ObjectLibrary.h"

namespace DerydocaEngine::Dgui
{

	std::shared_ptr<Resources::Resource> DerydocaEngine::Dgui::ResourcePicker(
		const std::string label,
		const std::string& resourceName,
		const Resources::ResourceType resourceType
	)
	{
		bool result = false;
		static bool opened = false;
		static int selectedIndex = -1;
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
		ImGui::SetNextWindowSize({ 250, 400 }, ImGuiCond_FirstUseEver);
		if (ImGui::BeginPopupModal("Resource Picker", NULL, ImGuiWindowFlags_None))
		{
			if (!opened)
			{
				// Initialize the list
				opened = true;
				resources = ObjectLibrary::getInstance().getResourcesOfType(resourceType);
			}
	
			auto cra = ImGui::GetContentRegionAvail();
			if (ImGui::ListBoxHeader("List", ImVec2(cra.x, cra.y - 30)))
			{
				ImGui::Selectable("None", selectedIndex < 0, ImGuiSelectableFlags_AllowDoubleClick);
				if (ImGui::IsItemClicked())
				{
					selectedIndex = -1;
					if (ImGui::IsMouseDoubleClicked(0))
					{
						result = true;
					}
				}
				for (int i = 0; i < resources.size(); i++)
				{
					ImGui::Selectable(resources[i]->getName().c_str(), selectedIndex == i, ImGuiSelectableFlags_AllowDoubleClick);
					if (ImGui::IsItemClicked())
					{
						selectedIndex = i;
						if (ImGui::IsMouseDoubleClicked(0))
						{
							result = true;
						}
					}
				}
				ImGui::ListBoxFooter();
			}
	
			if (ImGui::Button("Cancel", { 60, 0 }))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 100);
			if (ImGui::Button("Select", { 100, 0 }))
			{
				result = true;
			}
	
			if (result == true)
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
		if (result && selectedIndex >= 0)
		{
			return resources[selectedIndex];
		}
		else
		{
			return nullptr;
		}
	}

}