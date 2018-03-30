#include "Camera.h"
#include "CameraManager.h"
#include "Shader.h"
#include "MatrixStack.h"
#include "DisplayManager.h"
#include "MeshResource.h"
#include "glm\glm.hpp"
#include "Shader.h"

using namespace glm;

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
	MeshResource* quadResource = (MeshResource*)ObjectLibrary::getInstance().getResource("136a5d0f-51d7-4f3c-857c-0497de142a71");
	if (quadResource != nullptr)
	{
		m_quad = (Mesh*)quadResource->getResourceObject();
	}

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

void Camera::deserialize(YAML::Node node)
{
	m_fov = node["fov"].as<float>();
	m_zNear = node["zNear"].as<float>();
	m_zFar = node["zFar"].as<float>();

	YAML::Node renderTextureNode = node["RenderTexture"];
	if (renderTextureNode)
	{
		int width = renderTextureNode["Width"].as<int>();
		int height = renderTextureNode["Height"].as<int>();
		m_renderTexture = new RenderTexture(width, height);

		m_postProcessShader = loadResource<Shader*>(renderTextureNode, "PostProcessShader");
	}

	recalcPerspectiveMatrix();
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
		(GLint)(textureW * m_displayRect->getX()),
		(GLint)(textureH * m_displayRect->getY()),
		(GLint)(textureW * m_displayRect->getWidth()),
		(GLint)(textureH * m_displayRect->getHeight()));
	glEnable(GL_DEPTH_TEST);
	clear();
	gameObject->preRender();
	gameObject->render(m_matrixStack);

	// Postprocessing happens here
	if (m_renderTexture != nullptr && m_postProcessShader != nullptr)
	{
		glDisable(GL_DEPTH_TEST);

		// Load the shader with matricies that will transform the quad to take up the entire buffer
		mat4 m = mat4(1.0);
		mat4 v = mat4(1.0);
		mat4 p = mat4(1.0);
		mat4 mv = v * m;
		mat4 mvp = p * v * m;

		m_postProcessShader->bind();
		
		m_postProcessShader->setMat4("ModelViewMatrix", mv);
		m_postProcessShader->setMat3("NormalMatrix", mat3(mv[0], vec3(mv[1]), vec3(mv[2])));
		m_postProcessShader->setMat4("MVP", mvp);

		m_postProcessShader->setInt("Width", m_renderTexture->getWidth());
		m_postProcessShader->setInt("Height", m_renderTexture->getHeight());
		m_postProcessShader->setTexture("RenderTex", 0, m_renderTexture);

		// Render the full-buffer quad
		m_postProcessShader->renderMesh(m_quad);
	}
}
