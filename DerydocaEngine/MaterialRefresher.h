#pragma once
#include "GameComponent.h"
#include "MeshRenderer.h"
#include "Keyboard.h"
//#include <fileapi.h>
#include <windows.h>

class MaterialRefresher : public GameComponent
{
public:
	MaterialRefresher(Keyboard* keyboard, MeshRenderer* meshRenderer);
	~MaterialRefresher();

	void init();
	void update(float deltaTime);
	void refreshMaterial();
private:
	bool isShaderSourceUpdated();
	MeshRenderer* m_meshRenderer;
	Keyboard* m_keyboard;
	std::string m_shaderLoadPath;
	std::string m_vertexShaderPath;
	std::string m_fragmentShaderPath;
	FILETIME m_vertexShaderModifiedTime;
	FILETIME m_fragmentShaderModifiedTime;
};

