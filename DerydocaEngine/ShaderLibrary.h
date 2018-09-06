#pragma once
#include "Shader.h"
#include <map>
#include <string>
#include <boost/uuid/uuid.hpp>

using namespace std;
using shaderMap = map<uuid, Shader*>;
using shaderPathMap = map<string, uuid>;

class ShaderLibrary
{
public:
	static ShaderLibrary& getInstance()
	{
		static ShaderLibrary instance;
		return instance;
	}
	void operator=(ObjectLibrary const&) = delete;

	Shader* find(uuid const& shaderId);
	Shader* find(string const& shaderPath);

	void registerShaderName(string const& shaderPath, uuid const& shaderUuid);
private:
	ShaderLibrary() {}
	~ShaderLibrary() {}
	ShaderLibrary(ShaderLibrary const&) {}

	shaderMap m_shaders;
	shaderPathMap m_shaderPaths;
};