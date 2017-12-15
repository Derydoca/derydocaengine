#include "Camera.h"
#include "CameraManager.h"
#include "Shader.h"
#include "MatrixStack.h"

Camera::Camera(float fov, float aspect, float zNear, float zFar) :
	m_fov(fov),
	m_aspect(aspect),
	m_zNear(zNear),
	m_zFar(zFar)
{
	recalcPerspectiveMatrix();
	CameraManager::getInstance().addCamera(this);

	m_skybox = new Skybox();
	m_matrixStack = new MatrixStack();
}

Camera::~Camera()
{
	// TODO: Figure out why deleting the matrix stack is creating a compiler error
	//delete(m_matrixStack);
	delete(m_skybox);
	CameraManager::getInstance().removeCamera(this);
}

void Camera::init()
{
	m_transform = getGameObject()->getTransform();
}

void Camera::setFov(float fov)
{
	m_fov = fov;
	recalcPerspectiveMatrix();
}

void Camera::clear()
{
	glClear(GL_DEPTH_BUFFER_BIT);

	switch (m_clearMode)
	{
	case ColorClear:
		glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT);
		break;
	case SkyboxClear:
		assert(m_skyboxMaterial);

		m_skyboxMaterial->bind();
		m_skyboxMaterial->getShader()->update(getRotationProjection());
		m_skybox->getMesh()->draw();
		break;
	default:
		break;
	}
}

void Camera::renderRoot(GameObject* gameObject)
{
	clear();
	gameObject->render(m_matrixStack);
}
