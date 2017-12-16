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
	m_displayRect = new Rectangle();
}

Camera::~Camera()
{
	delete(m_displayRect);
	// TODO: Figure out why deleting the matrix stack is creating a compiler error
	//delete(m_matrixStack);
	delete(m_skybox);
	CameraManager::getInstance().removeCamera(this);
}

void Camera::init()
{
	m_transform = getGameObject()->getTransform();
}

void Camera::setDisplayRect(float x, float y, float w, float h)
{
	m_displayRect->setX(x);
	m_displayRect->setY(x);
	m_displayRect->setWidth(x);
	m_displayRect->setHeight(x);
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
	int textureW, textureH = 1;

	if (m_renderTexture)
	{
		m_renderTexture->bindAsRenderTexture();
		textureW = m_renderTexture->getWidth();
		textureH = m_renderTexture->getHeight();
	}
	else
	{
		m_display->bindAsRenderTarget();
		textureW = m_display->getWidth();
		textureH = m_display->getHeight();
	}

	glViewport(
		textureW * m_displayRect->getX(),
		textureH * m_displayRect->getY(),
		textureW * m_displayRect->getWidth(),
		textureH * m_displayRect->getHeight());
	clear();
	gameObject->render(m_matrixStack);
}
