#include "ShaderResourceSerializer.h"
#include "ShaderLibrary.h"
#include "ShaderResource.h"

void * ShaderResourceSerializer::deserialize(Resource * const& resource)
{
	ShaderResource* shaderResource = (ShaderResource*)resource;
	Shader* shader = new Shader(shaderResource->getRawShaderName());
	return shader;
}

ResourceType ShaderResourceSerializer::getResourceType()
{
	return ResourceType::ShaderResourceType;
}
