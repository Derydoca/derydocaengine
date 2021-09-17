#include "EnginePch.h"
#include "Files\Serializers\ShaderFileSerializer.h"
#include "Resources\ShaderResource.h"
#include "Rendering\ShaderLibrary.h"

namespace DerydocaEngine::Files::Serializers {

	std::vector<std::shared_ptr<Resources::Resource>> ShaderFileSerializer::generateResources(const std::string& assetPath)
	{
		auto resources = std::vector<std::shared_ptr<Resources::Resource>>();

		auto resource = std::make_shared<Resources::ShaderResource>();
		resource->generateAndSetId();
		resources.push_back(resource);

		return resources;
	}

	Files::FileType ShaderFileSerializer::getFileType()
	{
		return Files::FileType::ShaderFileType;
	}

	void ShaderFileSerializer::postLoadInitialize(std::shared_ptr<Resources::Resource> resource)
	{
		auto shaderResource = std::static_pointer_cast<Resources::ShaderResource>(resource);
		std::filesystem::path vertexShaderPath(shaderResource->getVertexShaderLocation());

		// Get the path that this exists for
		std::filesystem::path shaderDirectory = vertexShaderPath.parent_path();

		// Remove the extension from the file name so we can append to all other shader types
		std::filesystem::path rawShaderName = vertexShaderPath.replace_extension();
		shaderResource->setRawShaderName(rawShaderName.string());

		// If a fragment shader exists
		std::filesystem::path fragmentShaderPath = rawShaderName.append(".fs");
		if (exists(fragmentShaderPath))
		{
			// Set the path to the fragment shader
			shaderResource->setFragmentShaderSource(fragmentShaderPath.string());
		}

		// If a geometry shader exists
		std::filesystem::path geometryShaderPath = rawShaderName.append(".gs");
		if (exists(geometryShaderPath))
		{
			// Set the path to the geometry shader
			shaderResource->setGeometryShaderSource(geometryShaderPath.string());
		}

		// Store the shader name in the library
		Rendering::ShaderLibrary::getInstance().registerShaderName(shaderResource->getRawShaderName(), shaderResource->getId());
	}

}
