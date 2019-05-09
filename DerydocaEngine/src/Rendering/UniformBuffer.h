#pragma once
#include "EnginePch.h"
#include "Rendering\GraphicsAPI.h"

namespace DerydocaEngine::Rendering
{

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
			GraphicsAPI::createUniformBuffer(m_ubo, nullptr, sizeof(T), GraphicsAPI::USAGE_PATTERN_DYNAMIC_DRAW);
		}

		void destroy()
		{
			if (m_ubo)
			{
				GraphicsAPI::deleteUniformBuffer(m_ubo);
			}
		}

		void uploadData(const T* lightData)
		{
			GraphicsAPI::updateUniformBuffer(m_ubo, lightData, sizeof(T));
		}

		unsigned int getRendererId()
		{
			return m_ubo;
		}

	private:
		unsigned int m_ubo;
	};

}
