#include "MaterialRefresher.h"
#include "Material.h"
#include "Shader.h"

MaterialRefresher::MaterialRefresher()
{
}

MaterialRefresher::~MaterialRefresher()
{
}

bool getLastModifiedTime(std::string filePath, std::time_t &time)
{
	if (!boost::filesystem::exists(filePath))
	{
		return false;
	}

	time = boost::filesystem::last_write_time(filePath);
	return true;
}

void MaterialRefresher::init()
{
	m_meshRenderer = getComponent<MeshRenderer>();
	Material* material = nullptr;
	if (m_meshRenderer != nullptr)
	{
		m_usingMeshRenderer = true;
		material = m_meshRenderer->getMaterial();
	}
	else
	{
		m_usingMeshRenderer = false;
		m_tessMeshRenderer = getComponent<TessellatedMeshRenderer>();
		material = m_tessMeshRenderer->getMaterial();
	}
	assert(material);

	Shader* shader = material->getShader();
	assert(shader);

	m_shaderLoadPath = shader->GetLoadPath();
	m_vertexShaderPath = shader->GetVertexShaderPath();
	m_fragmentShaderPath = shader->GetFragmentShaderPath();
	m_geometryShaderPath = shader->GetGeometryShaderPath();
	m_tessEvalShaderPath = shader->GetTessellationEvaluationShaderPath();
	m_tessControlShaderPath = shader->GetTesslleationControlShaderPath();

	// Store the initial modified time
	m_vertexShaderExists = getLastModifiedTime(m_vertexShaderPath.c_str(), m_vertexShaderModifiedTime);
	m_fragmentShaderExists = getLastModifiedTime(m_fragmentShaderPath.c_str(), m_fragmentShaderModifiedTime);
	m_geometryShaderExists = getLastModifiedTime(m_geometryShaderPath.c_str(), m_geometryShaderModifiedTime);
	m_tessEvalShaderExists = getLastModifiedTime(m_tessEvalShaderPath.c_str(), m_tessEvalShaderModifiedTime);
	m_tessControlShaderExists = getLastModifiedTime(m_tessControlShaderPath.c_str(), m_tessControlShaderModifiedTime);
}

void MaterialRefresher::update(float deltaTime)
{
	if (isShaderSourceUpdated()) {
		refreshMaterial();
	}
}

void MaterialRefresher::refreshMaterial()
{
	// Remember the last modified time for later
	m_vertexShaderExists = getLastModifiedTime(m_vertexShaderPath.c_str(), m_vertexShaderModifiedTime);
	m_fragmentShaderExists = getLastModifiedTime(m_fragmentShaderPath.c_str(), m_fragmentShaderModifiedTime);
	m_geometryShaderExists = getLastModifiedTime(m_geometryShaderPath.c_str(), m_geometryShaderModifiedTime);
	m_tessEvalShaderExists = getLastModifiedTime(m_tessEvalShaderPath.c_str(), m_tessEvalShaderModifiedTime);
	m_tessControlShaderExists = getLastModifiedTime(m_tessControlShaderPath.c_str(), m_tessControlShaderModifiedTime);

	printf("Unloading the previous material.\n");
	Material* mat = m_usingMeshRenderer ? m_meshRenderer->getMaterial() : m_tessMeshRenderer->getMaterial();
	Shader* oldShader = mat->getShader();
	delete(oldShader);

	printf("Reloading new material.\n");
	Shader* newShader = new Shader(m_shaderLoadPath);
	mat->setShader(newShader);
	printf("Material reload finished.\n");
}

bool MaterialRefresher::isShaderSourceUpdated()
{
	time_t tempFileTime;

	// Check the vertex shader
	if (
		m_vertexShaderExists &&
		getLastModifiedTime(m_vertexShaderPath.c_str(), tempFileTime) &&
		difftime(tempFileTime, m_vertexShaderModifiedTime) != 0) {
		return true;
	}

	// Check the fragment shader
	if (
		m_fragmentShaderExists &&
		getLastModifiedTime(m_fragmentShaderPath.c_str(), tempFileTime) &&
		difftime(tempFileTime, m_fragmentShaderModifiedTime) != 0) {
		return true;
	}

	// Check the geometry shader
	if (
		m_geometryShaderExists &&
		getLastModifiedTime(m_geometryShaderPath.c_str(), tempFileTime) &&
		difftime(tempFileTime, m_geometryShaderModifiedTime) != 0) {
		return true;
	}

	// Check the tesselation evaluation shader
	if (
		m_tessEvalShaderExists &&
		getLastModifiedTime(m_tessEvalShaderPath.c_str(), tempFileTime) &&
		difftime(tempFileTime, m_tessEvalShaderModifiedTime) != 0) {
		return true;
	}

	// Check the tesselation control shader
	if (
		m_tessControlShaderExists &&
		getLastModifiedTime(m_tessControlShaderPath.c_str(), tempFileTime) &&
		difftime(tempFileTime, m_tessControlShaderModifiedTime) != 0) {
		return true;
	}

	// If neither were changed, return false
	return false;
}
