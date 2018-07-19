#pragma once
#include "GameComponent.h"
#include "MeshRenderer.h"
#include "TessellatedMeshRenderer.h"
#include <windows.h>

/*
This will monitor the material source code and recompile/reapply the shader to the object that it is attached to whenever it changes.
*/
class MaterialRefresher : public GameComponent
{
public:
	GENINSTANCE(MaterialRefresher);
	MaterialRefresher();
	~MaterialRefresher();

	void init();
	void update(float deltaTime);
	void refreshMaterial();
private:
	bool isShaderSourceUpdated();
	bool m_usingMeshRenderer = true;
	MeshRenderer* m_meshRenderer;
	TessellatedMeshRenderer* m_tessMeshRenderer;
	bool m_vertexShaderExists = false;
	bool m_fragmentShaderExists = false;
	bool m_geometryShaderExists = false;
	bool m_tessEvalShaderExists = false;
	bool m_tessControlShaderExists = false;
	std::string m_shaderLoadPath;
	std::string m_vertexShaderPath;
	std::string m_fragmentShaderPath;
	std::string m_geometryShaderPath;
	std::string m_tessEvalShaderPath;
	std::string m_tessControlShaderPath;
	FILETIME m_vertexShaderModifiedTime;
	FILETIME m_fragmentShaderModifiedTime;
	FILETIME m_geometryShaderModifiedTime;
	FILETIME m_tessEvalShaderModifiedTime;
	FILETIME m_tessControlShaderModifiedTime;
};

