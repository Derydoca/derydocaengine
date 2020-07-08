#pragma once
#include "Components\GameComponent.h"
#include "Rendering\Texture.h"
#include "Components\MeshRenderer.h"
#include "Resources\TextureResource.h"

namespace DerydocaEngine::Ext
{

	class ImageProjector : public Components::GameComponent, Components::SelfRegister<ImageProjector>
	{
	public:
		GENINSTANCE(ImageProjector);

		ImageProjector();
		~ImageProjector();

		virtual void init();
		virtual void deserialize(const YAML::Node& compNode);
		virtual void update(const float deltaTime);

		void inline setFocalPoint(glm::vec3 const& focalPoint) { m_FocalPoint = focalPoint; }
		void inline setUpVector(glm::vec3 const& upVector) { m_UpVector = upVector; }
		void updateShader();

		float& getFov() { return m_FOV; }

		SERIALIZE_FUNC_DEFINITIONS;
	private:
		glm::vec3 m_FocalPoint;
		glm::vec3 m_UpVector;
		float m_FOV;
		float m_ZNear;
		float m_ZFar;
		ResourceRef<Resources::TextureResource> m_ProjectorTexture;
		std::vector<std::shared_ptr<Components::MeshRenderer>> m_MeshRenderers;

		glm::mat4 m_ProjectorMatrix;

		void updateProjectionMatrix();
		void setProjectionGraphic();
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Ext::ImageProjector, 0);
