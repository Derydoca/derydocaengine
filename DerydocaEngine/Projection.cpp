#include "Projection.h"

#include <glm/gtc/constants.hpp>

namespace DerydocaEngine::Rendering
{
	void Projection::recalculateProjectionMatrix()
	{
		switch (m_projectionMode)
		{
		case ProjectionMode::Orthographic:
			m_projectionMatrix = glm::mat4(); //glm::ortho(-m_orthoSize, m_orthoSize, -m_orthoSize, m_orthoSize, m_zNear, m_zFar);
			break;
		case ProjectionMode::Perspective:
			m_projectionMatrix = glm::perspective(m_fov * (glm::pi<float>() / 180), m_aspect, m_zNear, m_zFar);
			break;
		default:
			m_projectionMatrix = glm::mat4();
			break;
		}
	}

}
