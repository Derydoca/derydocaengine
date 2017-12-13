#pragma once
#include "Shader.h"

class Material
{
public:
	Material();
	~Material();

	inline void setShader(Shader* shader) { m_shader = shader; }
	inline Shader* getShader() const { return m_shader; }
private:
	Shader* m_shader;
};

