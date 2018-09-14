#pragma once

#include <GL/glew.h>
#include <yaml-cpp/yaml.h>
#include "Color.h"
#include "GameComponent.h"
#include "GameObject.h"
#include "MatrixStack.h"
#include "Projection.h"

struct Rectangle;
class Display;
namespace DerydocaEngine::Rendering {
	class Material;
	class Mesh;
}
class RenderTexture;
class Shader;
class Skybox;

namespace DerydocaEngine::Components
{

	/*
	Object used for rendering the world to screen.
	*/
	class Camera : public GameComponent
	{
	public:
		GENINSTANCE(Camera);

		/* Types of screen clear methods when rendering a camera. */
		enum ClearMode {
			/* Do not clear the screen. */
			NoClear,

			/* Clears the screen with a solid color. */
			ColorClear,

			/* Clears the screen with a skybox. */
			SkyboxClear
		};

		enum RenderingMode {
			Forward = 0,
			Deferred = 1
		};

		Camera();
		Camera(float const& fov, float const& aspect, float const& zNear, float const& zFar);
		~Camera();

		inline RenderTexture* getRenderTexture() const { return m_renderTexture; }

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

		/*
		Sets the skybox material

		@skyboxMaterial Material to use for the skybox
		*/
		void setSkybox(Rendering::Material* const& skyboxMaterial) { m_skyboxMaterial = skyboxMaterial; }

		/*
		Renders the root node through this camera

		@root Root game object to render all of its children
		*/
		void renderRoot(GameObject* const& root);

		void setDisplay(Display* const& display);
		Display* getDisplay() { return m_display; }
		float getDisplayWidth();
		float getDisplayHeight();
		void setRenderTexture(RenderTexture* const& renderTexture) { m_renderTexture = renderTexture; }
		void init();
		void setDisplayRect(float const& x, float const& y, float const& w, float const& h);
		void setRenderingMode(RenderingMode const& mode);
		void resize(int const& width, int const& height);

		void createGBufTex(GLenum const& textureUnit, GLenum const& format, GLuint & texid, int const& width, int const& height);

		void setProjectionMode(ProjectionMode const& mode);
		void setOrthoSize(float const& size);
		float getOrthoSize(float const& size) { return m_orthoSize; }
		void deserialize(YAML::Node const& node);
		Shader* getPostProcessShader() const;
		Rendering::Material* getPostProcessMaterial() { return m_postProcessMaterial; }
		Projection getProjection() const { return m_projection; }
	private:
		Transform * m_transform;
		Color m_clearColor;
		Skybox* m_skybox;
		ClearMode m_clearMode = NoClear;
		RenderingMode m_renderingMode;
		Rendering::Material* m_skyboxMaterial;
		Rendering::MatrixStack m_matrixStack;
		RenderTexture* m_renderTexture;
		Display* m_display;
		Rectangle* m_displayRect;
		Rendering::Mesh* m_quad;
		Rendering::Material* m_postProcessMaterial;
		float m_orthoSize = 10.0f;
		GLuint m_deferredFBO;
		GLuint m_gbuffDepth, m_gbuffPos, m_gbuffNorm, m_gbuffColor;
		Shader* m_deferredRendererCompositor;
		Projection m_projection;

		void clear();
		void setIdentityMatricies(Shader* const& shader);
	};

}