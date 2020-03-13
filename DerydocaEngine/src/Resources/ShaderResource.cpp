#include "EnginePch.h"
#include "ShaderResource.h"
#include "Rendering\ShaderLibrary.h";
#include <boost\filesystem.hpp>

namespace fs = boost::filesystem;

void DerydocaEngine::Resources::ShaderResource::postLoadInitialize()
{
	if (getSourceFilePath().find("basicShader") != std::string::npos)
	{
		__debugbreak;
	}

	fs::path vertexShaderPath(getVertexShaderLocation());

	// Get the path that this exists for
	fs::path shaderDirectory = vertexShaderPath.parent_path();

	// Remove the extension from the file name so we can append to all other shader types
	fs::path rawShaderName = change_extension(vertexShaderPath, "");
	setRawShaderName(rawShaderName.string());

	// If a fragment shader exists
	fs::path fragmentShaderPath = rawShaderName.append(".fs");
	if (exists(fragmentShaderPath))
	{
		// Set the path to the fragment shader
		setFragmentShaderSource(fragmentShaderPath.string());
	}

	// If a geometry shader exists
	fs::path geometryShaderPath = rawShaderName.append(".gs");
	if (exists(geometryShaderPath))
	{
		// Set the path to the geometry shader
		setGeometryShaderSource(geometryShaderPath.string());
	}

	// Store the shader name in the library
	Rendering::ShaderLibrary::getInstance().registerShaderName(getRawShaderName(), getId());
}
