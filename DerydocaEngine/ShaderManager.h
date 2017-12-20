#pragma once
#include "Shader.h"

class ShaderManager
{
public:
	static ShaderManager& getInstance() {
		static ShaderManager instance;
		return instance;
	}

	void operator=(ShaderManager const&) = delete;

	Shader* getShadowShader() { return m_shadowShader; }
	Shader* getPhong() { return m_phong; }
private:
	ShaderManager();
	~ShaderManager();

	void initializeStaticShaders();

	ShaderManager(ShaderManager const&);

	static ShaderManager m_instance;

	Shader* m_shadowShader;
	Shader* m_phong;
};

