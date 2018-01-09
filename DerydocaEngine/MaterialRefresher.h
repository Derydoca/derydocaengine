#pragma once
#include "GameComponent.h"
#include "MeshRenderer.h"
#include <windows.h>

/*
This will monitor the material source code and recompile/reapply the shader to the object that it is attached to whenever it changes.
*/
class MaterialRefresher : public GameComponent
{
public:
	MaterialRefresher(MeshRenderer* meshRenderer);
	~MaterialRefresher();

	void init();
	void update(float deltaTime);
	void refreshMaterial();
private:
	bool isShaderSourceUpdated();
	MeshRenderer* m_meshRenderer;
	std::string m_shaderLoadPath;
	std::string m_vertexShaderPath;
	std::string m_fragmentShaderPath;
	FILETIME m_vertexShaderModifiedTime;
	FILETIME m_fragmentShaderModifiedTime;
};

