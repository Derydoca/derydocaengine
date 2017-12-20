#include "ShaderManager.h"
#include "Shader.h"

ShaderManager::ShaderManager()
{
	initializeStaticShaders();
}

ShaderManager::~ShaderManager()
{
	if (m_shadowShader) delete m_shadowShader;
	if (m_phong) delete m_phong;
}

void ShaderManager::initializeStaticShaders()
{
	m_shadowShader = new Shader("../res/shadowShader");
	m_phong = new Shader("../res/phong");
}