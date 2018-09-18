#include "ShaderResourceSerializer.h"
#include "ShaderLibrary.h"
#include "ShaderResource.h"

namespace DerydocaEngine::Resources::Serializers
{

	void * ShaderResourceSerializer::deserialize(Resource * const& resource)
	{
		ShaderResource* shaderResource = (ShaderResource*)resource;
		DerydocaEngine::Rendering::Shader* shader = new DerydocaEngine::Rendering::Shader(shaderResource->getRawShaderName());
		return shader;
	}

	ResourceType ShaderResourceSerializer::getResourceType()
	{
		return ResourceType::ShaderResourceType;
	}

}
