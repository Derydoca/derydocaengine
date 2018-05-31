#pragma once

#include "Transform.h"
#include "GameComponent.h"
#include "GameObject.h"
#include "Skybox.h"
#include "Color.h"
#include "Material.h"
#include "RenderTexture.h"
#include "Display.h"
#include "Rectangle.h"
#include "Projection.h"
#include "MatrixStack.h"

class MatrixStack;

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
	Camera(float fov, float aspect, float zNear, float zFar);
	~Camera();

	inline RenderTexture* getRenderTexture() const { return m_renderTexture; }

	/*
	Sets the clear mode

	@clearMode Clear mode to switch to
	*/
	void setClearMode(ClearMode clearMode) { m_clearMode = clearMode; }

	/*
	Sets the clear color

	@clearColor Color to use when clearing the camera
	*/
	void setClearColor(Color clearColor) { m_clearColor = clearColor; }

	/*
	Sets the skybox material

	@skyboxMaterial Material to use for the skybox
	*/
	void setSkybox(Material* skyboxMaterial) { m_skyboxMaterial = skyboxMaterial; }

	/*
	Renders the root node through this camera

	@root Root game object to render all of its children
	*/
	void renderRoot(GameObject* root);

	void setDisplay(Display* display) { m_display = display; }
	Display* getDisplay() { return m_display; }
	float getDisplayWidth() { return m_renderTexture != nullptr ? m_renderTexture->getWidth() : m_display->getWidth(); }
	float getDisplayHeight() { return m_renderTexture != nullptr ? m_renderTexture->getHeight() : m_display->getHeight(); }
	void setRenderTexture(RenderTexture* renderTexture) { m_renderTexture = renderTexture; }
	void init();
	void setDisplayRect(float x, float y, float w, float h);
	void setRenderingMode(RenderingMode mode);

	void createGBufTex(GLenum textureUnit, GLenum format, GLuint &texid, int width, int height);

	void setProjectionMode(ProjectionMode mode);
	void setOrthoSize(float size);
	float getOrthoSize(float size) { return m_orthoSize; }
	void deserialize(YAML::Node node);
	Shader* getPostProcessShader() const { return m_postProcessShader; }
	Projection getProjection() const { return m_projection; }
private:
	Transform* m_transform;
	Color m_clearColor;
	Skybox* m_skybox;
	ClearMode m_clearMode = NoClear;
	RenderingMode m_renderingMode;
	Material* m_skyboxMaterial;
	MatrixStack m_matrixStack;
	RenderTexture* m_renderTexture;
	Display* m_display;
	Rectangle* m_displayRect;
	Mesh* m_quad;
	Shader* m_postProcessShader;
	float m_orthoSize = 10.0f;
	GLuint m_deferredFBO;
	GLuint m_gbuffDepth, m_gbuffPos, m_gbuffNorm, m_gbuffColor;
	Shader* m_deferredRendererCompositor;
	Projection m_projection;

	void clear();
	void setIdentityMatricies(Shader* shader);
};

