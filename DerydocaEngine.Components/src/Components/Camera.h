#pragma once

#include <yaml-cpp/yaml.h>
#include "Color.h"
#include "Components\GameComponent.h"
#include "GameObject.h"
#include "Rendering\MatrixStack.h"
#include "Rendering\Projection.h"
#include "Rendering\RenderingMode.h"
#include "Scenes\Scene.h"

struct Rectangle;
namespace DerydocaEngine::Rendering {
	class Display;
	class Material;
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

		inline std::shared_ptr<Rendering::RenderTexture> getRenderTexture() const { return m_renderTexture; }

		/*
		Sets the clear mode

		@clearMode Clear mode to switch to
		*/
		void setClearMode(ClearMode const& clearMode) { m_clearMode = clearMode; }

		/*
		Sets the clear color

		@clearColor Color to use when clearing the camera
		*/
		void setClearColor(Color const& clearColor) { m_clearColor = clearColor; }

		void setProjection(Rendering::Projection projection)
		{
			m_projection = projection;
		}

		/*
		Sets the skybox material

		@skyboxMaterial Material to use for the skybox
		*/
		void setSkybox(std::shared_ptr<Rendering::Material> skyboxMaterial) { m_skyboxMaterial = skyboxMaterial; }

		/*
		Renders the root node through this camera

		@root Root game object to render all of its children
		*/
		void renderScenesToActiveBuffer(const std::vector<std::shared_ptr<Scenes::Scene>> scenes, int textureW, int textureH);
		void renderToAttachedRenderTexture(const std::vector<std::shared_ptr<Scenes::Scene>> scenes);

		void setDisplay(Rendering::Display* const& display);
		Rendering::Display* getDisplay() { return m_display; }
		float getDisplayWidth();
		float getDisplayHeight();
		void setRenderTexture(std::shared_ptr<Rendering::RenderTexture> renderTexture) { m_renderTexture = renderTexture; }
		void setDisplayRect(float const& x, float const& y, float const& w, float const& h);
		void resize(int const& width, int const& height);
		virtual void init();
		virtual void preDestroy();

		void createGBufTex(unsigned int const& textureUnit, unsigned int const& format, unsigned int & texid, int const& width, int const& height);

		void setProjectionMode(Rendering::ProjectionMode const& mode);
		void deserialize(const YAML::Node& node);
		std::shared_ptr<Rendering::Shader> getPostProcessShader() const;
		std::shared_ptr<Rendering::Material> getPostProcessMaterial() { return m_postProcessMaterial; }
		Rendering::Projection& getProjection() { return m_projection; }
		Rendering::Projection getProjection() const { return m_projection; }
		Color& getClearColor() { return m_clearColor; }
		ClearMode getClearMode() { return m_clearMode; }
		Rectangle& getDisplayRect() { return m_displayRect; }
	private:
		Camera(bool registerWithManager);

		void clear();
		void setIdentityMatricies(std::shared_ptr<Rendering::Shader> shader);

	private:
		std::shared_ptr<Components::Transform> m_transform;
		Color m_clearColor;
		std::shared_ptr<Rendering::Skybox> m_skybox;
		ClearMode m_clearMode;
		std::shared_ptr<Rendering::Material> m_skyboxMaterial;
		std::shared_ptr<Rendering::MatrixStack> m_matrixStack;
		std::shared_ptr<Rendering::RenderTexture> m_renderTexture;
		std::shared_ptr<Rendering::RenderTexture> m_renderTextureDeferred;
		Rendering::Display* m_display;
		Rectangle m_displayRect;
		std::shared_ptr<Rendering::Mesh> m_quad;
		std::shared_ptr<Rendering::Material> m_postProcessMaterial;
		std::shared_ptr<Rendering::Shader> m_deferredRendererCompositor;
		Rendering::Projection m_projection;
		bool m_registerWithManager;
	};

}