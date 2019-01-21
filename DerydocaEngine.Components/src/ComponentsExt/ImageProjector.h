#pragma once
#include "Components\GameComponent.h"
#include "Rendering\Texture.h"
#include "Components\MeshRenderer.h"

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

		void inline setFocalPoint(glm::vec3 const& focalPoint) { m_focalPoint = focalPoint; }
		void inline setUpVector(glm::vec3 const& upVector) { m_upVector = upVector; }

		float& getFov() { return m_fov; }
	private:
		glm::vec3 m_focalPoint;
		glm::vec3 m_upVector;
		glm::mat4 m_projectorMatrix;
		float m_fov;
		float m_zNear;
		float m_zFar;
		std::shared_ptr<Rendering::Texture> m_projectorTexture;
		std::vector<std::shared_ptr<Components::MeshRenderer>> m_meshRenderers;

		void updateProjectionMatrix();
		void setProjectionGraphic();
	};

}
