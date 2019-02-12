#pragma once

#include "Color.h"
#include "Components\GameComponent.h"
#include "Rendering\Material.h"
#include "Rendering\MatrixStack.h"
#include "Rendering\Projection.h"
#include "Scenes\Scene.h"

namespace DerydocaEngine::Rendering {
	class Display;
	class Mesh;
	class RenderTexture;
	class Shader;
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

		virtual void deserialize(const YAML::Node& node) override;
		virtual void init() override;
		virtual void preDestroy() override;

		void renderScenesToActiveBuffer(const std::vector<std::shared_ptr<Scenes::Scene>> scenes, int textureW, int textureH);
		void renderToAttachedRenderTexture(const std::vector<std::shared_ptr<Scenes::Scene>> scenes);
		void resize(int const& width, int const& height);

		Color& getClearColor() { return m_clearColor; }
		ClearMode getClearMode() { return m_clearMode; }
		float getDisplayHeight();
		float getDisplayWidth();
		Rectangle& getDisplayRect() { return m_displayRect; }
		std::shared_ptr<Rendering::Material> getPostProcessMaterial() { return m_postProcessMaterial; }
		std::shared_ptr<Rendering::Shader> getPostProcessShader() const { return m_postProcessMaterial ? m_postProcessMaterial->getShader() : nullptr; }
		Rendering::Projection& getProjection() { return m_projection; }
		Rendering::Projection getProjection() const { return m_projection; }
		std::shared_ptr<Rendering::RenderTexture> getRenderTexture() const { return m_renderTexture; }

		void setClearColor(Color const& clearColor) { m_clearColor = clearColor; }
		void setClearMode(ClearMode const& clearMode) { m_clearMode = clearMode; }
		void setDisplayRect(float const& x, float const& y, float const& w, float const& h);
		void setProjection(Rendering::Projection projection)
		{
			m_projection = projection;
		}
		void setProjectionMode(Rendering::ProjectionMode const& mode);
		void setRenderTexture(std::shared_ptr<Rendering::RenderTexture> renderTexture) { m_renderTexture = renderTexture; }
		void setSkybox(std::shared_ptr<Rendering::Material> skyboxMaterial) { m_skyboxMaterial = skyboxMaterial; }

	private:
		Camera(bool registerWithManager);

		void clear();
		void setIdentityMatricies(std::shared_ptr<Rendering::Shader> shader);

	private:
		Color m_clearColor;
		std::shared_ptr<Rendering::Skybox> m_skybox;
		ClearMode m_clearMode;
		std::shared_ptr<Rendering::Material> m_skyboxMaterial;
		std::shared_ptr<Rendering::MatrixStack> m_matrixStack;
		std::shared_ptr<Rendering::RenderTexture> m_renderTexture;
		std::shared_ptr<Rendering::RenderTexture> m_renderTextureDeferred;
		Rectangle m_displayRect;
		std::shared_ptr<Rendering::Mesh> m_quad;
		std::shared_ptr<Rendering::Material> m_postProcessMaterial;
		std::shared_ptr<Rendering::Shader> m_deferredRendererCompositor;
		Rendering::Projection m_projection;
		bool m_registerWithManager;
	};

}