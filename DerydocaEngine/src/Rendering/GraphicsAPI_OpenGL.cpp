#include "EnginePch.h"

#if OPENGL

#include "GraphicsAPI.h"

namespace DerydocaEngine::Rendering
{

	void GraphicsAPI::init()
	{
		GLenum status = glewInit();

		if (status != GLEW_OK) {
			std::cerr << "Unable to initialize OpenGL loader!\n";
		}
	}

}

#endif
