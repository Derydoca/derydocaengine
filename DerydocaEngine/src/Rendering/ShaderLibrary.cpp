#include "EnginePch.h"
#include "Rendering\ShaderLibrary.h"

#include <iostream>
#include "ObjectLibrary.h"
#include "Rendering\Shader.h"
#include "Resources\ShaderResource.h"

namespace DerydocaEngine::Rendering
{

	ShaderLibrary::ShaderLibrary() :
		m_shaders(),
		m_shaderPaths(),
		m_errorShader()
	{
	}

	ShaderLibrary::~ShaderLibrary()
	{
	}

	std::shared_ptr<Resources::ShaderResource> Rendering::ShaderLibrary::findResource(const uuids::uuid& shaderId)
	{
		// Convert the resource object to a shader resource object
		std::shared_ptr<Resources::ShaderResource> shaderResource = ObjectLibrary::getInstance().getResource<Resources::ShaderResource>(shaderId);
		if (!shaderResource)
		{
			D_LOG_ERROR("The resource with ID of '{}' is not a shader.", shaderId.to_string());
			return nullptr;
		}
		return shaderResource;
	}

	std::shared_ptr<Shader> Rendering::ShaderLibrary::find(const uuids::uuid& shaderId)
	{
		// Look through the list of already loaded shaders
		auto it = m_shaders.find(shaderId);

		// If the shader was found, return it
		if (it != m_shaders.end())
		{
			return it->second;
		}

		// Convert the resource object to a shader resource object
		std::shared_ptr<Resources::ShaderResource> shaderResource = ObjectLibrary::getInstance().getResource<Resources::ShaderResource>(shaderId);
		if (!shaderResource)
		{
			D_LOG_ERROR("The resource with ID of '{}' is not a shader.", shaderId.to_string());
			return nullptr;
		}

		// Load the shader from the resource object
		auto shader = std::static_pointer_cast<Shader>(shaderResource->getResourceObjectPointer());

		// Add it to the library
		m_shaders[shaderId] = shader;

		// Return the shader
		return shader;
	}

	std::shared_ptr<Shader> Rendering::ShaderLibrary::find(const std::string& shaderPath)
	{
		// Find the uuid associated with the path
		auto it = m_shaderPaths.find(shaderPath);

		// If it could not be found, return with a null shader
		if (it == m_shaderPaths.end())
		{
			D_LOG_ERROR("Unable to find a shader uuid associated with the path '{}'.", shaderPath);
			return nullptr;
		}

		// Otherwise, use the UUID to load the shader
		return find(it->second);
	}

	void ShaderLibrary::initialize()
	{
		m_errorShader = ObjectLibrary::getInstance().getResource<Resources::ShaderResource>("3a8405f2-e16f-4aa7-bef0-5644d7d19cdd");
	}

	void Rendering::ShaderLibrary::registerShaderName(std::string const& shaderPath, uuids::uuid const& shaderUuid)
	{
		m_shaderPaths.insert(std::pair<std::string, uuids::uuid>(shaderPath, shaderUuid));
	}

}
