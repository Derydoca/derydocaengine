#pragma once
#include "EditorPch.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Animation\AnimationData.h"
#include "Resources\AnimationResource.h"

namespace DerydocaEngine::Editor::Inspector::ResourceInspectors
{

	class AnimationResourceInspector : public InspectorRenderer
	{
		virtual void render(std::shared_ptr<Object> object)
		{
			auto resource = std::static_pointer_cast<Resources::AnimationResource>(object);

			ImGui::Text(resource->getName().c_str());

			auto animation = std::static_pointer_cast<Animation::AnimationData>(resource->getResourceObjectPointer());
			std::ostringstream val;
			val << animation->getDuration() << " seconds";
			ImGui::LabelText(val.str().c_str(), "Duration");
		}
	};

}
