#include "Camera.h"
#include "CameraManager.h"
#include "Shader.h"
#include "MatrixStack.h"
#include "DisplayManager.h"

Camera::Camera()
{
	CameraManager::getInstance().addCamera(this);

	m_skybox = new Skybox();
	m_matrixStack = new MatrixStack();
	m_displayRect = new Rectangle(0, 0, 1, 1);

	setDisplay(DisplayManager::getInstance().getDisplay(0));
	m_aspect = m_display->getAspectRatio();
	setClearMode(ClearMode::ColorClear);
	setClearColor(Color(0.5, 0.0, 0.0));
}

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

void Camera::setProjectionMode(ProjectionMode mode)
{
	m_projectionMode = mode;
	recalcPerspectiveMatrix();
}

void Camera::setOrthoSize(float size)
{
	m_orthoSize = size;
	recalcPerspectiveMatrix();
}

bool Camera::deserialize(YAML::Node node)
{
	m_fov = node["fov"].as<float>();
	m_zNear = node["zNear"].as<float>();
	m_zFar = node["zFar"].as<float>();

	recalcPerspectiveMatrix();

	return true;
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

inline void Camera::recalcPerspectiveMatrix()
{
	switch (m_projectionMode) {
	case Orthographic:
		m_projectionMatrix = glm::mat4(); //glm::ortho(-m_orthoSize, m_orthoSize, -m_orthoSize, m_orthoSize, m_zNear, m_zFar);
		break;
	case Perspective:
		m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_zNear, m_zFar);
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
