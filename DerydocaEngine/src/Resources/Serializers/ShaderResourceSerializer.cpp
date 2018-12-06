#include "Resources\Serializers\ShaderResourceSerializer.h"

#include "Rendering\Shader.h"
#include "Rendering\ShaderLibrary.h"
#include "Resources\ShaderResource.h"

namespace DerydocaEngine::Resources::Serializers
{

	std::shared_ptr<void> ShaderResourceSerializer::deserializePointer(std::shared_ptr<Resource> resource)
	{
		auto shaderResource = std::static_pointer_cast<ShaderResource>(resource);
		auto shader = std::make_shared<Rendering::Shader>(shaderResource->getRawShaderName());
		return shader;
	}

}
