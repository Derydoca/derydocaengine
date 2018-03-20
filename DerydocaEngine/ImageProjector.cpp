#include "ImageProjector.h"
#include "GameObject.h"
#include "YamlTools.h"
#include "Shader.h"

ImageProjector::ImageProjector()
{
}


ImageProjector::~ImageProjector()
{
}

void ImageProjector::init()
{
	
}

void ImageProjector::deserialize(YAML::Node compNode)
{
	YAML::Node focalPointNode = compNode["focalPoint"];
	if (focalPointNode)
	{
		m_focalPoint = focalPointNode.as<vec3>();
	}

	YAML::Node upVectorNode = compNode["upVector"];
	if (upVectorNode)
	{
		m_upVector = upVectorNode.as<vec3>();
	}
	else
	{
		m_upVector = vec3(0.0f, 1.0f, 0.0);
	}

	YAML::Node fovNode = compNode["fov"];
	if (fovNode)
	{
		m_fov = fovNode.as<float>();
	}
	else
	{
		m_fov = 30.0f;
	}

	YAML::Node zNearNode = compNode["zNear"];
	if (zNearNode)
	{
		m_zNear = zNearNode.as<float>();
	}
	else
	{
		m_zNear = 1.0f;
	}

	YAML::Node zFarNode = compNode["zFar"];
	if (zFarNode)
	{
		m_zFar = zFarNode.as<float>();
	}
	else
	{
		m_zFar = 1000.0f;
	}

	//m_texture = loadResource<Texture*>(compNode, "texture");

	m_meshRenderers = loadComponents<MeshRenderer*>(compNode, "affectedMeshRenderers");
}

void ImageProjector::update(float deltaTime)
{
	if (m_dirty)
	{
		updateProjectionMatrix();

		// Update all shaders with the new projection matrix
		for (MeshRenderer* const& meshRenderer : m_meshRenderers)
		{
			Material* mat = meshRenderer->getMaterial();
			if (!mat)
			{
				continue;
			}
			Shader* shader = mat->getShader();
			if (!shader)
			{
				continue;
			}
			shader->setMat4("ProjectorMatrix", m_projectionMatrix);
		}
	}
}

void ImageProjector::updateProjectionMatrix()
{
	// Safely calculate the aspect ratio based on the texture assigned to this component
	float aspectRatio = 1.0;
	if (m_texture != nullptr && m_texture->getHeight() != 0)
	{
		aspectRatio = (float)m_texture->getWidth() / (float)m_texture->getHeight();
	}

	// Create the projection matrix
	mat4 projView = lookAt(getGameObject()->getTransform()->getWorldPos(), m_focalPoint, m_upVector);
	mat4 projProj = perspective(m_fov, aspectRatio, m_zNear, m_zFar);
	mat4 projScaleTrans = glm::translate(vec3(0.5f)) * glm::scale(vec3(0.5f));

	// Store it locally
	m_projectionMatrix = projScaleTrans * projProj * projView;

	// Clear our dirty flag
	m_dirty = false;
}
