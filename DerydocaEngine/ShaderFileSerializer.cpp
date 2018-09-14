#include "ShaderFileSerializer.h"
#include "ShaderResource.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/convenience.hpp>
#include "ShaderLibrary.h"

namespace fs = boost::filesystem;

namespace DerydocaEngine::FileSerializers {

	YAML::Node ShaderFileSerializer::generateResourceNodes(std::string const& filePath)
	{
		YAML::Node resources;

		YAML::Node shaderResource;
		shaderResource["ID"] = generateUuid();

		resources.push_back(shaderResource);

		return resources;
	}

	FileType ShaderFileSerializer::getFileType()
	{
		return FileType::ShaderFileType;
	}

	Resource * ShaderFileSerializer::loadResourceFromMeta(YAML::Node const& resourceNode)
	{
		ShaderResource* resource = new ShaderResource();

		resource->setType(ShaderResourceType);

		return resource;
	}

	void ShaderFileSerializer::postLoadInitialize(Resource* const& resource)
	{
		ShaderResource* shaderResource = (ShaderResource*)resource;
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
