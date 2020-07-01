#include "EngineComponentsPch.h"
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
	}

	void ImageProjector::init()
	{
		updateShader();
	}

	SERIALIZE_FUNC_LOAD(archive, ImageProjector)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_FocalPoint),
			SERIALIZE(m_UpVector),
			SERIALIZE(m_FOV),
			SERIALIZE(m_ZNear),
			SERIALIZE(m_ZFar),
			SERIALIZE(m_ProjectorTexture)
			//SERIALIZE(m_MeshRenderers)
		);

		// TODO: Set the wrap mode on the texture to CLAMP
		// Load the projector texture with flags to disable all wrapping
		//Rendering::TextureParameters textureParams;
		//textureParams.setWrapModeS(Rendering::TextureWrapMode::CLAMP_TO_BORDER);
		//textureParams.setWrapModeT(Rendering::TextureWrapMode::CLAMP_TO_BORDER);
		//auto texture = std::static_pointer_cast<Rendering::Texture>(m_ProjectorTexture->getResourceObjectPointer());
		//texture->SetWrapMode();

		D_LOG_WARN("The ImageProjector component is intentionally broken. A mechanism is needed to store reference to affected mesh renderers.");

		setProjectionGraphic();
	}

	SERIALIZE_FUNC_SAVE(archive, ImageProjector)
	{
		archive(SERIALIZE_BASE(DerydocaEngine::Components::GameComponent),
			SERIALIZE(m_FocalPoint),
			SERIALIZE(m_UpVector),
			SERIALIZE(m_FOV),
			SERIALIZE(m_ZNear),
			SERIALIZE(m_ZFar),
			SERIALIZE(m_ProjectorTexture)
			//SERIALIZE(m_MeshRenderers)
		);
	}

	void ImageProjector::deserialize(const YAML::Node& compNode)
	{
		YAML::Node focalPointNode = compNode["focalPoint"];
		if (focalPointNode)
		{
			m_FocalPoint = focalPointNode.as<glm::vec3>();
		}

		YAML::Node upVectorNode = compNode["upVector"];
		if (upVectorNode)
		{
			m_UpVector = upVectorNode.as<glm::vec3>();
		}
		else
		{
			m_UpVector = glm::vec3(0.0f, 1.0f, 0.0);
		}

		YAML::Node fovNode = compNode["fov"];
		if (fovNode)
		{
			m_FOV = fovNode.as<float>();
		}
		else
		{
			m_FOV = 30.0f;
		}

		YAML::Node zNearNode = compNode["zNear"];
		if (zNearNode)
		{
			m_ZNear = zNearNode.as<float>();
		}
		else
		{
			m_ZNear = 1.0f;
		}

		YAML::Node zFarNode = compNode["zFar"];
		if (zFarNode)
		{
			m_ZFar = zFarNode.as<float>();
		}
		else
		{
			m_ZFar = 1000.0f;
		}

		// Load the projector texture with flags to disable all wrapping
		Rendering::TextureParameters textureParams;
		textureParams.setWrapModeS(Rendering::TextureWrapMode::CLAMP_TO_BORDER);
		textureParams.setWrapModeT(Rendering::TextureWrapMode::CLAMP_TO_BORDER);
		auto projectorTextureResource = getResource<Resources::TextureResource>(compNode, "texture");
		m_ProjectorTexture.Set(projectorTextureResource);
		//m_ProjectorTexture = std::make_shared<Rendering::Texture>(projectorTextureResource->getSourceFilePath(), &textureParams);

		// Load references to all mesh renderers this shader affects
		//m_meshRenderers = loadComponents<Components::MeshRenderer>(compNode, "affectedMeshRenderers");
		D_LOG_WARN("ImageProjector.cpp::84 was intentionally commented out. This will not be fixed until the serialization is changed to use the Cereal library.");

		setProjectionGraphic();
	}

	void ImageProjector::update(const float deltaTime)
	{
		updateShader();
	}

	void ImageProjector::updateShader()
	{
		updateProjectionMatrix();

		// Update all shaders with the new projection matrix
		for (std::shared_ptr<Components::MeshRenderer> meshRenderer : m_MeshRenderers)
		{
			std::shared_ptr<Rendering::Material> mat = meshRenderer->getMaterial();
			if (!mat)
			{
				continue;
			}
			mat->setMat4("ProjectorMatrix", m_ProjectorMatrix);
		}
	}

	void ImageProjector::updateProjectionMatrix()
	{
		// Safely calculate the aspect ratio based on the texture assigned to this component
		float aspectRatio = 1.0;
		if (m_ProjectorTexture)
		{
			auto texture = std::static_pointer_cast<Rendering::Texture>(m_ProjectorTexture->getResourceObjectPointer());
			if (texture->getHeight() != 0)
			{
				aspectRatio = (float)texture->getWidth() / (float)texture->getHeight();
			}
		}

		// Create the projection matrix
		glm::mat4 projView = lookAt(getGameObject()->getTransform()->getWorldPosition(), m_FocalPoint, m_UpVector);
		glm::mat4 projProj = glm::perspective(glm::radians(m_FOV), aspectRatio, m_ZNear, m_ZFar);
		glm::mat4 projScaleTrans = glm::translate(glm::mat4(), glm::vec3(0.5f)) * glm::scale(glm::mat4(), glm::vec3(0.5f));

		// Store it locally
		m_ProjectorMatrix = projScaleTrans * projProj * projView;
	}

	void ImageProjector::setProjectionGraphic()
	{
		// Update all shaders with the new projection matrix
		for (std::shared_ptr<DerydocaEngine::Components::MeshRenderer> meshRenderer : m_MeshRenderers)
		{
			std::shared_ptr<Rendering::Material> mat = meshRenderer->getMaterial();
			if (!mat)
			{
				continue;
			}
			auto texture = std::static_pointer_cast<Rendering::Texture>(m_ProjectorTexture->getResourceObjectPointer());
			mat->setTexture("ProjectorTex", texture);
		}
	}

}
