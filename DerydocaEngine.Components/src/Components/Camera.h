#pragma once

#include "Color.h"
#include "Components\GameComponent.h"
#include "Rendering\Material.h"
#include "Rendering\Projection.h"
#include "Rendering\RenderingMode.h"
#include "Scenes\Scene.h"
#include "Resources\ShaderResource.h"
#include "Resources\TextureResource.h"
#include "Resources\MaterialResource.h"

namespace DerydocaEngine::Rendering {
	class Display;
	class Mesh;
	class RenderTexture;
	class Skybox;
}

namespace DerydocaEngine::Components
{

	/*
	Object used for rendering the world to screen.
	*/
	class Camera : public GameComponent, SelfRegister<Camera>
	{
	public:
		GENINSTANCE(Camera);
		SERIALIZE_FUNC_DEFINITIONS;

		/* Tag used for Camera constructor to instruct it to not be registered with CameraManager */
		class NoRegister {};

		/* Types of screen clear methods when rendering a camera. */
		enum ClearMode {
			/* Do not clear the screen. */
			NoClear,

			/* Clears the screen with a solid color. */
			ColorClear,

			/* Clears the screen with a skybox. */
			SkyboxClear
		};

		Camera();
		Camera(NoRegister);
		~Camera();

		virtual void init() override;
		virtual void preDestroy() override;

		void renderScenesToActiveBuffer(const std::vector<std::shared_ptr<Scenes::Scene>> scenes, int textureW, int textureH);
		void renderToAttachedRenderTexture(const std::vector<std::shared_ptr<Scenes::Scene>> scenes);
		void resize(int const& width, int const& height);

		Color& getClearColor() { return m_ClearColor; }
		ClearMode getClearMode() { return m_ClearMode; }
		float getDisplayHeight();
		float getDisplayWidth();
		Rectangle& getDisplayRect() { return m_DisplayRect; }
		std::shared_ptr<Rendering::Material> getPostProcessMaterial() { return m_PostProcessMaterial; }
		std::shared_ptr<Rendering::Shader> getPostProcessShader() const { return m_PostProcessMaterial ? m_PostProcessMaterial->getShader() : nullptr; }
		Rendering::Projection& getProjection() { return m_Projection; }
		Rendering::Projection getProjection() const { return m_Projection; }
		std::shared_ptr<Rendering::RenderTexture> getRenderTexture() const { return m_RenderTexture; }

		void setClearColor(Color const& clearColor) { m_ClearColor = clearColor; }
		void setClearMode(ClearMode const& clearMode) { m_ClearMode = clearMode; }
		void setDisplayRect(float const& x, float const& y, float const& w, float const& h);
		void setProjection(Rendering::Projection projection)
		{
			m_Projection = projection;
		}
		void setProjectionMode(Rendering::ProjectionMode const& mode);
		void setRenderTexture(std::shared_ptr<Rendering::RenderTexture> renderTexture) { m_RenderTexture = renderTexture; }
		void setSkybox(std::shared_ptr<Resources::MaterialResource> skyboxMaterialResource) { m_SkyboxMaterial.Set(skyboxMaterialResource); }

	private:
		Camera(bool registerWithManager);

		void clear();
		void setIdentityMatricies(std::shared_ptr<Rendering::Shader> shader);

		std::shared_ptr<Rendering::Shader> GetDeferredCompositorShader()
		{
			return std::static_pointer_cast<Rendering::Shader>(m_DeferredRendererCompositor->getResourceObjectPointer());
		}
		std::shared_ptr<Rendering::Shader> GetPostPricessShader()
		{
			return std::static_pointer_cast<Rendering::Shader>(m_PostProcessShader->getResourceObjectPointer());
		}
		std::shared_ptr<Rendering::Material> GetSkyboxMaterial()
		{
			return std::static_pointer_cast<Rendering::Material>(m_SkyboxMaterial->getResourceObjectPointer());
		}

	private:
		Rendering::Projection m_Projection;
		Rendering::RenderingMode m_RenderingMode;
		float2 m_RenderTextureSize;
		ResourceRef<Resources::ShaderResource> m_PostProcessShader;
		ClearMode m_ClearMode;
		Color m_ClearColor;
		ResourceRef<Resources::ShaderResource> m_DeferredRendererCompositor;
		ResourceRef<Resources::MaterialResource> m_SkyboxMaterial;

		std::shared_ptr<Rendering::Skybox> m_Skybox;
		std::shared_ptr<Rendering::RenderTexture> m_RenderTexture;
		std::shared_ptr<Rendering::RenderTexture> m_RenderTextureDeferred;
		Rectangle m_DisplayRect;
		std::shared_ptr<Rendering::Mesh> m_Quad;
		std::shared_ptr<Rendering::Material> m_PostProcessMaterial;
		bool m_RegisterWithManager;
	};

}

REGISTER_SERIALIZED_TYPE(DerydocaEngine::Components::Camera, 0);
