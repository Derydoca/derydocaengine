#include "EnginePch.h"
#include "Rendering\Projection.h"

#include <glm/gtc/constants.hpp>

namespace DerydocaEngine::Rendering
{
	void Projection::recalculateProjectionMatrix()
	{
		switch (m_ProjectionMode)
		{
		case ProjectionMode::Orthographic:
			m_ProjectionMatrix = glm::mat4(); //glm::ortho(-m_orthoSize, m_orthoSize, -m_orthoSize, m_orthoSize, m_zNear, m_zFar);
			break;
		case ProjectionMode::Perspective:
			m_ProjectionMatrix = glm::perspective(m_FOV * (glm::pi<float>() / 180), m_Aspect, m_ZNear, m_ZFar);
			break;
		default:
			m_ProjectionMatrix = glm::mat4();
			break;
		}
	}

}
