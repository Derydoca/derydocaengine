#include "EnginePch.h"
#include "Resources\Serializers\ShaderResourceSerializer.h"

#include "Rendering\Shader.h"
#include "Rendering\ShaderLibrary.h"
#include "Resources\ShaderResource.h"

namespace DerydocaEngine::Resources::Serializers
{

	std::shared_ptr<void> GlslShaderResourceSerializer::deserializePointer(std::shared_ptr<Resource> resource)
	{
		auto shaderResource = std::static_pointer_cast<GlslShaderResource>(resource);
		auto shader = std::make_shared<Rendering::GlslShader>(shaderResource->getRawShaderName());
		return shader;
	}

}
