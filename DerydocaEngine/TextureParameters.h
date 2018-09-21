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

		void setWrapModeS(TextureWrapMode const& mode) { m_wrapS = mode; }
		void setWrapModeT(TextureWrapMode const& mode) { m_wrapT = mode; }

		unsigned int textureWrapModeToOpenGL(TextureWrapMode const& mode);
	private:
		TextureWrapMode m_wrapS;
		TextureWrapMode m_wrapT;
	};

}
