#pragma once
#include "EditorPch.h"
#include "Editor\Inspector\InspectorRendererFactory.h"
#include "Rendering\Shader.h"
#include "Resources\ShaderResource.h"

namespace DerydocaEngine::Editor::Inspector::ResourceInspectors
{

	class ShaderResourceInspector : public InspectorRenderer
	{
		virtual void render(std::shared_ptr<Object> object)
		{
			auto resource = std::static_pointer_cast<Resources::ShaderResource>(object);

			ImGui::Text("Shader");
			ImGui::Text(resource->getName().c_str());

			auto shader = std::static_pointer_cast<Rendering::Shader>(resource->getResourceObjectPointer());

			//bool hasVertexShader = shader->GetVertexShaderPath().length() > 0;
			//bool hasTessEvalShader = shader->GetTessellationEvaluationShaderPath().length() > 0;
			//bool hasTessControlShader = shader->GetTesslleationControlShaderPath().length() > 0;
			//bool hasGeometryShader = shader->GetGeometryShaderPath().length() > 0;
			//bool hasFragmentShader = shader->GetFragmentShaderPath().length() > 0;
			//ImGui::Checkbox("Has Vertex Shader",   &hasVertexShader);
			//ImGui::Checkbox("Has Tess Eval Shader", &hasTessEvalShader);
			//ImGui::Checkbox("Has Tess Control Shader", &hasTessControlShader);
			//ImGui::Checkbox("Has Geometry Shader", &hasGeometryShader);
			//ImGui::Checkbox("Has Fragment Shader", &hasFragmentShader);
		}
	};

}
