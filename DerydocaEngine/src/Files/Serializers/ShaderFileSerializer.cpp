#include "Files\Serializers\ShaderFileSerializer.h"
#include "ShaderResource.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/convenience.hpp>
#include "ShaderLibrary.h"

namespace fs = boost::filesystem;

namespace DerydocaEngine::Files::Serializers {

	YAML::Node ShaderFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node shaderResource;
		shaderResource["ID"] = generateUuid();

		resources.push_back(shaderResource);

		return resources;
	}

	Files::FileType ShaderFileSerializer::getFileType()
	{
		return Files::FileType::ShaderFileType;
	}

	Resources::Resource * ShaderFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		Resources::ShaderResource* resource = new Resources::ShaderResource();

		resource->setType(Resources::ShaderResourceType);

		return resource;
	}

	void ShaderFileSerializer::postLoadInitialize(Resources::Resource* const& resource)
	{
		Resources::ShaderResource* shaderResource = (Resources::ShaderResource*)resource;
		fs::path vertexShaderPath(shaderResource->getVertexShaderLocation());

		// Get the path that this exists for
		fs::path shaderDirectory = vertexShaderPath.parent_path();

		// Remove the extension from the file name so we can append to all other shader types
		fs::path rawShaderName = change_extension(vertexShaderPath, "");
		shaderResource->setRawShaderName(rawShaderName.string());

		// If a fragment shader exists
		fs::path fragmentShaderPath = rawShaderName.append(".fs");
		if (exists(fragmentShaderPath))
		{
			// Set the path to the fragment shader
			shaderResource->setFragmentShaderSource(fragmentShaderPath.string());
		}

		// If a geometry shader exists
		fs::path geometryShaderPath = rawShaderName.append(".gs");
		if (exists(geometryShaderPath))
		{
			// Set the path to the geometry shader
			shaderResource->setGeometryShaderSource(geometryShaderPath.string());
		}

		// Store the shader name in the library
		Rendering::ShaderLibrary::getInstance().registerShaderName(shaderResource->getRawShaderName(), shaderResource->getId());
	}

}
