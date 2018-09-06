#pragma once
#include <string>
#include "RenderTexture.h"

using namespace std;

struct RenderPass
{
public:
	RenderPass()
	{
		m_name = "";
		m_shaderSubroutineIndex = -1;
		m_renderTexture = nullptr;
	}

	RenderPass(string const& name) : RenderPass()
	{
		m_name = name;
	}

	RenderPass(string const& name, RenderTexture* const& renderTexture, string const& renderTextureName)
	{
		assert(renderTexture != nullptr);

		m_name = name;
		m_renderTexture = renderTexture;
		m_renderTextureName = renderTextureName;
	}

	RenderPass(RenderPass* const& pass, int const& index)
	{
		m_name = pass->getName();
		m_shaderSubroutineIndex = index;
		m_renderTexture = pass->getRenderTexture();
		m_renderTextureName = pass->getRenderTextureName();
	}

	string getName() const { return m_name; }
	int getShaderSubroutineIndex() const { return m_shaderSubroutineIndex; }
	RenderTexture* getRenderTexture() const { return m_renderTexture; }
	string getRenderTextureName() const { return m_renderTextureName; }

	bool hasRenderTextureAssigned() const { return m_renderTexture != nullptr; }
private:
	string m_name;
	int m_shaderSubroutineIndex;
	RenderTexture* m_renderTexture = nullptr;
	string m_renderTextureName;
};