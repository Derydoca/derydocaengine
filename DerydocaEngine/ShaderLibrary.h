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

	Shader* find(uuid shaderId);
	Shader* find(string shaderPath);

	void registerShaderName(string shaderPath, uuid shaderUuid);
private:
	ShaderLibrary() {}
	~ShaderLibrary() {}
	ShaderLibrary(ShaderLibrary const&) {}

	shaderMap m_shaders;
	shaderPathMap m_shaderPaths;
};