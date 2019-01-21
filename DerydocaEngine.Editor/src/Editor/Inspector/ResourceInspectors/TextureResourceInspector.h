#pragma once
#include "EditorPch.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Rendering\Texture.h"
#include "Resources\TextureResource.h"

namespace DerydocaEngine::Editor::Inspector::ResourceInspectors
{

	class TextureResourceInspector : public InspectorRenderer
	{
		virtual void render(std::shared_ptr<Object> object)
		{
			auto resource = std::static_pointer_cast<Resources::TextureResource>(object);

			ImGui::Text("Texture");
			ImGui::Text(resource->getName().c_str());

			auto texture = std::static_pointer_cast<Rendering::Texture>(resource->getResourceObjectPointer());

			ImGui::LabelText(std::to_string(texture->getWidth()).c_str(), "Width");
			ImGui::LabelText(std::to_string(texture->getHeight()).c_str(), "Height");
			if (texture->getWidth() > 0 && texture->getHeight() > 0)
			{
				float windowWidth = ImGui::GetWindowWidth();
				ImVec2 imgSize = ImVec2(texture->getWidth() * 1.0f, texture->getHeight() * 1.0f);
				if (imgSize.x > windowWidth)
				{
					imgSize.x = windowWidth;
					imgSize.y = texture->getHeight() * (windowWidth / texture->getWidth());
				}
				ImGui::Image((ImTextureID)(texture->getRendererId()), imgSize);
			}
		}
	};

}
