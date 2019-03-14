#include "EditorPch.h"
#include "ResourcePicker.h"
#include "ObjectLibrary.h"

namespace DerydocaEngine::Dgui
{

	bool DerydocaEngine::Dgui::ResourcePicker(
		const std::string label,
		std::shared_ptr<Resources::Resource> resource,
		std::shared_ptr<Resources::Resource>& selectedResource
	)
	{
		bool result = false;
		static bool opened = false;
		static int selectedIndex = -1;
		static std::vector<std::shared_ptr<Resources::Resource>> resources;

		// Button to open asset picker
		std::string resourceName = resource ? resource->getName() : "[None]";
		if (ImGui::Button(resourceName.c_str()))
		{
			ImGui::OpenPopup(label.c_str());
		}

		// Field name
		ImGui::SameLine();
		ImGui::Text(label.c_str());

		// Modal popup
		ImGui::SetNextWindowSize({ 250, 400 }, ImGuiCond_FirstUseEver);
		if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_None))
		{
			// Initialize the list the first time the window appears
			if (!opened)
			{
				opened = true;
				resources = ObjectLibrary::getInstance().getResourcesOfType(resource->getType());
			}

			// Display a selectable list of resources that match the resource type
			auto cra = ImGui::GetContentRegionAvail();
			if (ImGui::ListBoxHeader("List", ImVec2(cra.x, cra.y - 30)))
			{
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

			// Confirmation/Cancel buttons
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

		if (result)
		{
			selectedResource = resources[selectedIndex];
		}
		return result;
	}

	bool ResourcePicker(
		const std::string label,
		std::shared_ptr<Resources::Resource> resource,
		Resources::ResourceType resourceType,
		std::shared_ptr<Resources::Resource>& selectedResource
	)
	{
		bool result = false;
		static bool opened = false;
		static int selectedIndex = -1;
		static std::vector<std::shared_ptr<Resources::Resource>> resources;

		// Button to open asset picker
		std::string resourceName = resource ? resource->getName() : "[None]";
		if (ImGui::Button(resourceName.c_str()))
		{
			ImGui::OpenPopup(label.c_str());
		}

		// Field name
		ImGui::SameLine();
		ImGui::Text(label.c_str());

		// Modal popup
		ImGui::SetNextWindowSize({ 250, 400 }, ImGuiCond_FirstUseEver);
		if (ImGui::BeginPopupModal(label.c_str(), NULL, ImGuiWindowFlags_None))
		{
			// Initialize the list the first time the window appears
			if (!opened)
			{
				opened = true;
				resources = ObjectLibrary::getInstance().getResourcesOfType(resourceType);
			}

			// Display a selectable list of resources that match the resource type
			auto cra = ImGui::GetContentRegionAvail();
			if (ImGui::ListBoxHeader("List", ImVec2(cra.x, cra.y - 30)))
			{
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

			// Confirmation/Cancel buttons
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

		if (result)
		{
			selectedResource = resources[selectedIndex];
		}
		return result;
	}

}