#pragma once
#include <string>
#include <GL/glew.h>
#include <map>
#include "Transform.h"
#include "Camera.h"
#include "Color.h"
#include "Texture.h"
#include "Mesh.h"
#include "RenderPass.h"
#include "Projection.h"

namespace DerydocaEngine::Rendering
{

	class Shader
	{
	public:
		Shader(std::string const& fileName);
		Shader(std::string const& fileName, int const& count, const char *const * varyings);
		~Shader();

		void bind();
		void update(MatrixStack * const& matrixStack, Projection const& projection, Transform* const& trans);
		void update(glm::mat4 const& matrix);
		void updateViaActiveCamera(MatrixStack * const& matrixStack);

		void setFloat(std::string const& name, float const& val);
		void setFloatArray(std::string const& name, float* const& arrayLocation, unsigned int const& arrayLength);
		void setColorRGB(std::string const& name, Color const& color);
		void setColorRGBA(std::string const& name, Color const& color);
		void setInt(std::string const& name, int const& val);
		void setIntArray(std::string const& name, int* const& arrayLocation, unsigned int const& arrayLength);
		void setVec3(std::string const& name, glm::vec3 const& val);
		void setVec4(std::string const& name, glm::vec4 const& val);
		void setMat3(std::string const& name, glm::mat3 const& val);
		void setMat4(std::string const& name, glm::mat4 const& val);
		void setTexture(std::string const& name, int const& textureUnit, Texture* const& handle);
		void setTexture(std::string const& name, int const& textureUnit, GLenum const& textureType, GLuint const& handle);

		void clearFloat(std::string const& name);
		void clearFloatArray(std::string const& name, unsigned int const& arrayLength);
		void clearColorRGB(std::string const& name);
		void clearColorRGBA(std::string const& name);
		void clearInt(std::string const& name);
		void clearIntArray(std::string const& name, unsigned int const& arrayLength);
		void clearVec3(std::string const& name);
		void clearVec4(std::string const& name);
		void clearMat3(std::string const& name);
		void clearMat4(std::string const& name);
		void clearTexture(std::string const& name, int const& textureUnit, GLenum const& textureType);

		std::string GetLoadPath() const { return m_loadPath; }
		std::string GetVertexShaderPath() const { return m_loadPath + ".vs"; }
		std::string GetTesslleationControlShaderPath() const { return m_loadPath + ".tcs"; }
		std::string GetTessellationEvaluationShaderPath() const { return m_loadPath + ".tes"; }
		std::string GetGeometryShaderPath() const { return m_loadPath + ".gs"; }
		std::string GetFragmentShaderPath() const { return m_loadPath + ".fs"; }

		GLuint getSubroutineIndex(GLuint const& program, std::string const& subroutineName);
		void setSubroutine(GLuint const& program, GLuint const& subroutineIndex);

		void setSubPasses(GLuint const& program, RenderPass* const& renderPasses, int const& numPasses);

		void renderMesh(Mesh* const& mesh, RenderTexture* const& renderTexture);
	private:
		static const unsigned int NUM_SHADERS = 5;
		Shader(Shader const& other) {}
		void operator=(Shader const& other) {}
		int getUniformName(std::string const& stringName);
		void setTransformFeedbackVaryings(int const& count, const char *const * varyings);

		enum {
			TRANSFORM_MVP = 0,
			TRANSFORM_MV = 1,
			TRANSFORM_NORMAL = 2,
			TRANSFORM_PROJECTION = 3,
			TRANSFORM_MODEL = 4,

			NUM_UNIFORMS
		};

		GLuint m_program;
		GLuint m_shaders[NUM_SHADERS];
		GLint m_uniforms[NUM_UNIFORMS];
		std::string m_loadPath;
		std::map<std::string, int> m_uniformLookup;
		int m_numPasses;
		RenderPass* m_renderPasses;
	};

}
