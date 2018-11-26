#include "ImageProjector.h"

#include "GameObject.h"
#include "Rendering\Material.h"
#include "ObjectLibrary.h"
#include "Resources\Resource.h"
#include "Rendering\Texture.h"
#include "Rendering\TextureParameters.h"
#include "Helpers\YamlTools.h"

namespace DerydocaEngine::Ext
{

	ImageProjector::ImageProjector()
	{
	}

	ImageProjector::~ImageProjector()
	{
		delete m_projectorTexture;
	}

	void ImageProjector::init()
	{

	}

	void ImageProjector::deserialize(YAML::Node const& compNode)
	{
		YAML::Node focalPointNode = compNode["focalPoint"];
		if (focalPointNode)
		{
			m_focalPoint = focalPointNode.as<glm::vec3>();
		}

		YAML::Node upVectorNode = compNode["upVector"];
		if (upVectorNode)
		{
			m_upVector = upVectorNode.as<glm::vec3>();
		}
		else
		{
			m_upVector = glm::vec3(0.0f, 1.0f, 0.0);
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
		Rendering::TextureParameters textureParams;
		textureParams.setWrapModeS(Rendering::TextureWrapMode::CLAMP_TO_BORDER);
		textureParams.setWrapModeT(Rendering::TextureWrapMode::CLAMP_TO_BORDER);
		Resources::Resource* projectorTextureResource = getResource(compNode, "texture");
		m_projectorTexture = new Rendering::Texture(projectorTextureResource->getSourceFilePath(), &textureParams);

		// Load references to all mesh renderers this shader affects
		m_meshRenderers = loadComponents<Components::MeshRenderer*>(compNode, "affectedMeshRenderers");

		setProjectionGraphic();
	}

	void ImageProjector::update(float const& deltaTime)
	{
		if (m_dirty)
		{
			updateProjectionMatrix();

			// Update all shaders with the new projection matrix
			for (Components::MeshRenderer* const& meshRenderer : m_meshRenderers)
			{
				Rendering::Material* mat = meshRenderer->getMaterial();
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
		glm::mat4 projView = lookAt(getGameObject()->getTransform()->getWorldPos(), m_focalPoint, m_upVector);
		glm::mat4 projProj = glm::perspective(glm::radians(m_fov), aspectRatio, m_zNear, m_zFar);
		glm::mat4 projScaleTrans = glm::translate(glm::mat4(), glm::vec3(0.5f)) * glm::scale(glm::mat4(), glm::vec3(0.5f));

		// Store it locally
		m_projectorMatrix = projScaleTrans * projProj * projView;

		// Clear our dirty flag
		m_dirty = false;
	}

	void ImageProjector::setProjectionGraphic()
	{
		// Update all shaders with the new projection matrix
		for (DerydocaEngine::Components::MeshRenderer* const& meshRenderer : m_meshRenderers)
		{
			Rendering::Material* mat = meshRenderer->getMaterial();
			if (!mat)
			{
				continue;
			}
			mat->setTexture("ProjectorTex", m_projectorTexture);
		}
	}

}
