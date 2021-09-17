#include "EnginePch.h"
#include "ShaderResource.h"
#include "Rendering\ShaderLibrary.h"

void DerydocaEngine::Resources::ShaderResource::postLoadInitialize()
{
	std::filesystem::path vertexShaderPath(getVertexShaderLocation());

	// Get the path that this exists for
	std::filesystem::path shaderDirectory = vertexShaderPath.parent_path();

	// Remove the extension from the file name so we can append to all other shader types
	std::filesystem::path rawShaderName = vertexShaderPath.replace_extension();
	setRawShaderName(rawShaderName.string());

	// If a fragment shader exists
	std::filesystem::path fragmentShaderPath = rawShaderName.append(".fs");
	if (exists(fragmentShaderPath))
	{
		// Set the path to the fragment shader
		setFragmentShaderSource(fragmentShaderPath.string());
	}

	// If a geometry shader exists
	std::filesystem::path geometryShaderPath = rawShaderName.append(".gs");
	if (exists(geometryShaderPath))
	{
		// Set the path to the geometry shader
		setGeometryShaderSource(geometryShaderPath.string());
	}

	// Store the shader name in the library
	Rendering::ShaderLibrary::getInstance().registerShaderName(getRawShaderName(), getId());
}
