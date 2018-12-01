#pragma once
#include <string>

namespace DerydocaEngine::Rendering {
	class RenderTexture;
}

namespace DerydocaEngine::Rendering
{

	struct RenderPass
	{
	public:
		RenderPass()
		{
			m_name = "";
			m_shaderSubroutineIndex = -1;
			m_renderTexture = nullptr;
		}

		RenderPass(std::string const& name) : RenderPass()
		{
			m_name = name;
		}

		RenderPass(std::string const& name, std::shared_ptr<RenderTexture> renderTexture, std::string const& renderTextureName)
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

		std::string getName() const { return m_name; }
		int getShaderSubroutineIndex() const { return m_shaderSubroutineIndex; }
		std::shared_ptr<RenderTexture> getRenderTexture() const { return m_renderTexture; }
		std::string getRenderTextureName() const { return m_renderTextureName; }

		bool hasRenderTextureAssigned() const { return m_renderTexture != nullptr; }
	private:
		std::string m_name;
		int m_shaderSubroutineIndex;
		std::shared_ptr<RenderTexture> m_renderTexture = nullptr;
		std::string m_renderTextureName;
	};

}
