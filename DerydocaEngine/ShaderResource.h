#pragma once
#include "Resource.h"
#include <string>

using namespace std;

struct ShaderResource : public Resource
{
public:
	ShaderResource() {}
	~ShaderResource() {}

	void setRawShaderName(string const& rawShaderName) { m_rawShaderName = rawShaderName; }
	void setFragmentShaderSource(string const& fragmentShaderSource) { m_fragmentShaderSource = fragmentShaderSource; }
	void setGeometryShaderSource(string const& geometryShaderSource) { m_geometryShaderSource = geometryShaderSource; }

	string getRawShaderName() const { return m_rawShaderName; }
	string getVertexShaderLocation() const { return m_sourceFilePath; }
	string getFragmentShaderSource() const { return m_fragmentShaderSource; }
	string getGeometryShaderSource() const { return m_geometryShaderSource; }
private:
	string m_rawShaderName;
	string m_fragmentShaderSource;
	string m_geometryShaderSource;
};