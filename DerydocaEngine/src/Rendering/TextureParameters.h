#pragma once

namespace DerydocaEngine::Rendering
{

	enum TextureWrapMode
	{
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER,
		MIRRORED_REPEAT,
		REPEAT,
		MIRROR_CLAMP_TO_EDGE
	};

	struct TextureParameters
	{
	public:
		TextureWrapMode getWrapModeS() const { return m_wrapS; }
		TextureWrapMode getWrapModeT() const { return m_wrapT; }

		void setWrapModeS(const TextureWrapMode mode) { m_wrapS = mode; }
		void setWrapModeT(const TextureWrapMode mode) { m_wrapT = mode; }

		static unsigned int textureWrapModeToOpenGL(const TextureWrapMode mode);
	private:
		TextureWrapMode m_wrapS;
		TextureWrapMode m_wrapT;
	};

}
