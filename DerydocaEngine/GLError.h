#pragma once
#include <iostream>

namespace DerydocaEngine::Debug
{
	const char* GLErrorToString(unsigned int err);

	void CheckOpenGLError(char* const& stmt, char* const& fname, int const& line);

#ifdef _DEBUG
#define GL_CHECK(stmt) do { \
stmt; \
DerydocaEngine::Debug::CheckOpenGLError(#stmt, __FILE__, __LINE__); \
} while (0)
#else
#define GL_CHECK(stmt) stmt
#endif

}
