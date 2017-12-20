#include "ShaderManager.h"
#include "Shader.h"

ShaderManager::ShaderManager()
{
	initializeStaticShaders();
}

ShaderManager::~ShaderManager()
{
	if (m_shadowShader) delete m_shadowShader;
}

void ShaderManager::initializeStaticShaders()
{
	m_shadowShader = new Shader("../res/shadowShader");
}