#include "ImageProjector.h"
#include "GameObject.h"
#include "YamlTools.h"
#include "ObjectLibrary.h"
#include "Resource.h"
#include "Texture.h"
#include "TextureParameters.h"

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

	// Load the projector texture with flags to disable all wrapping
	TextureParameters textureParams;
	textureParams.setWrapModeS(TextureWrapMode::CLAMP_TO_BORDER);
	textureParams.setWrapModeT(TextureWrapMode::CLAMP_TO_BORDER);
	Resource* projectorTextureResource = getResource(compNode, "texture");
	m_projectorTexture = new Texture(projectorTextureResource->getSourceFilePath(), &textureParams);

	// Load references to all mesh renderers this shader affects
	m_meshRenderers = loadComponents<MeshRenderer*>(compNode, "affectedMeshRenderers");

	setProjectionGraphic();
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
			mat->setMat4("ProjectorMatrix", m_projectorMatrix);
		}
	}
}

void ImageProjector::updateProjectionMatrix()
{
	// Safely calculate the aspect ratio based on the texture assigned to this component
	float aspectRatio = 1.0;
	if (m_projectorTexture != nullptr && m_projectorTexture->getHeight() != 0)
	{
		aspectRatio = (float)m_projectorTexture->getWidth() / (float)m_projectorTexture->getHeight();
	}

	// Create the projection matrix
	mat4 projView = lookAt(getGameObject()->getTransform()->getWorldPos(), m_focalPoint, m_upVector);
	mat4 projProj = perspective(glm::radians(m_fov), aspectRatio, m_zNear, m_zFar);
	mat4 projScaleTrans = glm::translate(mat4(), vec3(0.5f)) * glm::scale(mat4(), vec3(0.5f));

	// Store it locally
	m_projectorMatrix = projScaleTrans * projProj * projView;

	// Clear our dirty flag
	m_dirty = false;
}

void ImageProjector::setProjectionGraphic()
{
	// Update all shaders with the new projection matrix
	for (MeshRenderer* const& meshRenderer : m_meshRenderers)
	{
		Material* mat = meshRenderer->getMaterial();
		if (!mat)
		{
			continue;
		}
		mat->setTexture("ProjectorTex", m_projectorTexture);
	}
}
