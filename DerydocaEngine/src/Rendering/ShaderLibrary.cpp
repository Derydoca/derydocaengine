#include "EnginePch.h"
#include "Rendering\ShaderLibrary.h"

#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include "ObjectLibrary.h"
#include "Rendering\Shader.h"
#include "Resources\ShaderResource.h"

namespace DerydocaEngine::Rendering
{

	std::shared_ptr<Shader> Rendering::ShaderLibrary::find(const boost::uuids::uuid& shaderId)
	{
		// Look through the list of already loaded shaders
		auto it = m_shaders.find(shaderId);

		// If the shader was found, return it
		if (it != m_shaders.end())
		{
			return it->second;
		}

		//// Otherwise, try to load the shader
		//// Get the resource object
		//auto resource = ObjectLibrary::getInstance().getResource<Resources::ShaderResource>(shaderId);
		//if (resource == nullptr)
		//{
		//	std::cout << "Unable to find a resource with the ID of '" << shaderId << "'.\n";
		//	return nullptr;
		//}

		// Convert the resource object to a shader resource object
		std::shared_ptr<Resources::ShaderResource> shaderResource = ObjectLibrary::getInstance().getResource<Resources::ShaderResource>(shaderId);
		if (shaderResource == nullptr)
		{
			std::cout << "The resource with ID of '" << shaderId << "' is not a shader.\n";
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
			std::cout << "Unable to find a shader uuid associated with the path '" << shaderPath << "'.\n";
			return nullptr;
		}

		// Otherwise, use the UUID to load the shader
		return find(it->second);
	}

	void Rendering::ShaderLibrary::registerShaderName(std::string const& shaderPath, boost::uuids::uuid const& shaderUuid)
	{
		m_shaderPaths.insert(std::pair<std::string, boost::uuids::uuid>(shaderPath, shaderUuid));
	}

}
