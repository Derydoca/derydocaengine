#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Transform.h"

// TODO: Define the methods in the cpp file
class Camera
{
public:
	Camera(Transform* trans, float fov, float aspect, float zNear, float zFar) :
		m_transform(trans),
		m_fov(fov),
		m_aspect(aspect),
		m_zNear(zNear),
		m_zFar(zFar)
	{
		recalcPerspectiveMatrix();
	}
	~Camera();

	inline glm::mat4 getViewProjection() const { return m_perspective * m_transform->getModel(); }
	//inline Transform getTransform() { return m_transform; }

	inline void setFov(float fov)
	{
		m_fov = fov;
		recalcPerspectiveMatrix();
	};
private:
	float m_fov, m_aspect, m_zNear, m_zFar;
	glm::mat4 m_perspective;
	Transform* m_transform;

	inline void recalcPerspectiveMatrix() { m_perspective = glm::perspective(m_fov, m_aspect, m_zNear, m_zFar); }
};

