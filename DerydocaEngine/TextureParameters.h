#pragma once
#include <GL/glew.h>

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

	inline GLenum textureWrapModeToOpenGL(TextureWrapMode const& mode)
	{
		switch (mode)
		{
		case CLAMP_TO_EDGE:
			return GL_CLAMP_TO_EDGE;
		case CLAMP_TO_BORDER:
			return GL_CLAMP_TO_BORDER;
		case MIRRORED_REPEAT:
			return GL_MIRRORED_REPEAT;
		case REPEAT:
			return GL_REPEAT;
		case MIRROR_CLAMP_TO_EDGE:
			return GL_MIRROR_CLAMP_TO_EDGE;
		default:
			return GL_REPEAT;
		}
	};
private:
	TextureWrapMode m_wrapS;
	TextureWrapMode m_wrapT;
};