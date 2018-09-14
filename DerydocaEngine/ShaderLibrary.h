#pragma once
#include "Shader.h"
#include <map>
#include <string>
#include <boost/uuid/uuid.hpp>

using shaderMap = std::map<boost::uuids::uuid, DerydocaEngine::Rendering::Shader*>;
using shaderPathMap = std::map<std::string, boost::uuids::uuid>;

class ShaderLibrary
{
public:
	static ShaderLibrary& getInstance()
	{
		static ShaderLibrary instance;
		return instance;
	}
	void operator=(ObjectLibrary const&) = delete;

	DerydocaEngine::Rendering::Shader* find(boost::uuids::uuid const& shaderId);
	DerydocaEngine::Rendering::Shader* find(std::string const& shaderPath);

	void registerShaderName(std::string const& shaderPath, boost::uuids::uuid const& shaderUuid);
private:
	ShaderLibrary() {}
	~ShaderLibrary() {}
	ShaderLibrary(ShaderLibrary const&) {}

	shaderMap m_shaders;
	shaderPathMap m_shaderPaths;
};