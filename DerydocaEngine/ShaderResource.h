#pragma once
#include "Resource.h"
#include <string>

using namespace std;

struct ShaderResource : public Resource
{
public:
	ShaderResource() {}
	~ShaderResource() {}

	void setRawShaderName(string rawShaderName) { m_rawShaderName = rawShaderName; }
	void setFragmentShaderSource(string fragmentShaderSource) { m_fragmentShaderSource = fragmentShaderSource; }

	string getRawShaderName() const { return m_rawShaderName; }
	string getVertexShaderLocation() const { return m_sourceFilePath; }
	string getFragmentShaderSource() const { return m_fragmentShaderSource; }
private:
	string m_rawShaderName;
	string m_fragmentShaderSource;
};