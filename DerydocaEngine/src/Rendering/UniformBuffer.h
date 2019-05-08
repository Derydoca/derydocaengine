#pragma once
#include "EnginePch.h"
#include "..\Debug\GLError.h"

template<class T>
class UniformBuffer
{
public:
	UniformBuffer()
	{
		create();
	}

	~UniformBuffer()
	{
		destroy();
	}

	void create()
	{
		glGenBuffers(1, &m_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(T), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void destroy()
	{
		if (m_ubo)
		{
			glDeleteBuffers(1, &m_ubo);
		}
	}

	void uploadData(const T* lightData)
	{
		assert(m_ubo);

		GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, m_ubo));
		void* p;
		GL_CHECK(p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY));
		if (!p)
		{
			return;
		}
		memcpy(p, lightData, sizeof(T));
		GL_CHECK(glUnmapBuffer(GL_UNIFORM_BUFFER));
	}

	unsigned int getRendererId()
	{
		return m_ubo;
	}

private:
	unsigned int m_ubo;
};

