#pragma once
#include "EditorPch.h"
#include "Animation\Skeleton.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Resources\SkeletonResource.h"

namespace DerydocaEngine::Editor::Inspector::ResourceInspectors
{

	class SkeletonResourceInspector : public InspectorRenderer
	{
		virtual void render(std::shared_ptr<Object> object)
		{
			auto resource = std::static_pointer_cast<Resources::SkeletonResource>(object);

			ImGui::Text("Skeleton");
			ImGui::Text(resource->getName().c_str());

			auto skeleton = std::static_pointer_cast<Animation::Skeleton>(resource->getResourceObjectPointer());
			ImGui::LabelText(skeleton->getName().c_str(), "Name");
			ImGui::LabelText(std::to_string(skeleton->getNumBones()).c_str(), "Bone Count");
		}
	};

}
