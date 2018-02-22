#include "ShaderManager.h"
#include "Shader.h"
#include "ShaderLibrary.h"

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
	m_shadowShader = ShaderLibrary::getInstance().find(".\\engineResources\\shaders\\shadowShader");
	m_phong = ShaderLibrary::getInstance().find(".\\engineResources\\shaders\\phong");
}

// TODO: Kill this