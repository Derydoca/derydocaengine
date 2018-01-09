#include "MaterialRefresher.h"
#include "Material.h"
#include "Shader.h"

MaterialRefresher::MaterialRefresher(Keyboard* keyboard, MeshRenderer* meshRenderer) :
	m_keyboard(keyboard),
	m_meshRenderer(meshRenderer)
{
}


MaterialRefresher::~MaterialRefresher()
{
}

bool getLastModifiedTime(LPCSTR filePath, FILETIME &time)
{
	WIN32_FILE_ATTRIBUTE_DATA fileInfo = { 0 };
	if (GetFileAttributesEx(filePath, GetFileExInfoStandard, &fileInfo))
	{
		time = fileInfo.ftLastWriteTime;
		return true;
	}
	return false;
}

void MaterialRefresher::init()
{
	// Cache information necessary for hot-loading the shader
	Shader* shader = m_meshRenderer->getMaterial()->getShader();
	m_shaderLoadPath = shader->GetLoadPath();
	m_vertexShaderPath = shader->GetVertexShaderPath();
	m_fragmentShaderPath = shader->GetFragmentShaderPath();

	// Store the initial modified time
	getLastModifiedTime(m_vertexShaderPath.c_str(), m_vertexShaderModifiedTime);
	getLastModifiedTime(m_fragmentShaderPath.c_str(), m_fragmentShaderModifiedTime);
}

void MaterialRefresher::update(float deltaTime)
{
	if (m_keyboard->isKeyDownFrame(SDLK_m)) {
		refreshMaterial();
	}

	if (isShaderSourceUpdated()) {
		refreshMaterial();
	}
}

void MaterialRefresher::refreshMaterial()
{
	// Remember the last modified time for later
	getLastModifiedTime(m_vertexShaderPath.c_str(), m_vertexShaderModifiedTime);
	getLastModifiedTime(m_fragmentShaderPath.c_str(), m_fragmentShaderModifiedTime);

	printf("Unloading the previous material.\n");
	Material* mat = m_meshRenderer->getMaterial();
	Shader* oldShader = mat->getShader();
	delete(oldShader);

	printf("Reloading new material.\n");
	Shader* newShader = new Shader(m_shaderLoadPath);
	mat->setShader(newShader);
	printf("Material reload finished.\n");
}

bool MaterialRefresher::isShaderSourceUpdated()
{
	FILETIME tempFileTime;

	// Check the vertex shader
	if (
		getLastModifiedTime(m_vertexShaderPath.c_str(), tempFileTime) &&
		CompareFileTime(&tempFileTime, &m_vertexShaderModifiedTime) != 0) {
		return true;
	}

	// Check the fragment shader
	if (
		getLastModifiedTime(m_fragmentShaderPath.c_str(), tempFileTime) &&
		CompareFileTime(&tempFileTime, &m_fragmentShaderModifiedTime) != 0) {
		return true;
	}

	// If neither were changed, return false
	return false;
}
