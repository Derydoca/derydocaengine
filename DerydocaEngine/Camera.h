#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/color_space.hpp>
#include <glm/gtx/transform.hpp>
#include <list>
#include "Transform.h"
#include "GameComponent.h"
#include "GameObject.h"
#include "Skybox.h"
#include "Color.h"
#include "Material.h"

// TODO: Define the methods in the cpp file
class Camera : public GameComponent
{
public:
	Camera(float fov, float aspect, float zNear, float zFar);
	~Camera();

	enum ClearType {
		NoClear,
		ColorClear,
		SkyboxClear
	};

	inline glm::mat4 getViewProjection() const { return m_perspective * m_transform->getOtherModel(); }
	inline glm::mat4 getRotationProjection() const { return m_perspective * glm::mat4_cast(m_transform->getQuat()); }
	
	void init();

	inline void setFov(float fov)
	{
		m_fov = fov;
		recalcPerspectiveMatrix();
	};

	void setClearType(ClearType clearType) { m_clearType = clearType; }
	void setClearColor(Color clearColor) { m_clearColor = clearColor; }
	void setSkybox(Material* skyboxMaterial) { m_skyboxMaterial = skyboxMaterial; }

	void clear();
private:
	float m_fov, m_aspect, m_zNear, m_zFar;
	glm::mat4 m_perspective;
	Transform* m_transform;
	Color m_clearColor;
	Skybox* m_skybox;
	ClearType m_clearType = NoClear;
	Material* m_skyboxMaterial;

	inline void recalcPerspectiveMatrix() { m_perspective = glm::perspective(m_fov, m_aspect, m_zNear, m_zFar); }
};

