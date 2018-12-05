#include "Resources\Serializers\ShaderResourceSerializer.h"

#include "Rendering\Shader.h"
#include "Rendering\ShaderLibrary.h"
#include "Resources\ShaderResource.h"

namespace DerydocaEngine::Resources::Serializers
{

	std::shared_ptr<void> ShaderResourceSerializer::deserializePointer(Resource * const & resource)
	{
		ShaderResource* shaderResource = (ShaderResource*)resource;
		auto shader = std::make_shared<Rendering::Shader>(shaderResource->getRawShaderName());
		return shader;
	}

}
