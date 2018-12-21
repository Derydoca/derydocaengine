#include "EnginePch.h"
#include "Rendering\TextureParameters.h"

#include <GL/glew.h>

// TODO: Find a better way for abstraction. This is tacky.
unsigned int DerydocaEngine::Rendering::TextureParameters::textureWrapModeToOpenGL(const TextureWrapMode mode)
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
}
