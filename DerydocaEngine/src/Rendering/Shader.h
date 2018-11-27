#pragma once
#include <map>
#include <string>
#include <vector>
#include "Color.h"
#include "Rendering\Projection.h"

namespace DerydocaEngine {
	namespace Components {
		struct Transform;
	}
	namespace Rendering {
		class MatrixStack;
		class Mesh;
		struct RenderPass;
		class RenderTexture;
		class Texture;
	}
}

namespace DerydocaEngine::Rendering
{

	class Shader
	{
	public:
		Shader(std::string const& fileName);
		Shader(std::string const& fileName, int const& count, const char *const * varyings);
		~Shader();

		void bind();
		void update(std::shared_ptr<MatrixStack> const& matrixStack, Projection const& projection, Components::Transform* const& trans);
		void update(glm::mat4 const& matrix);
		void updateViaActiveCamera(std::shared_ptr<MatrixStack> const& matrixStack);

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
		void setMat4Array(std::string const& name, std::vector<glm::mat4> const& valArray);
		void setTexture(std::string const& name, int const& textureUnit, Texture* const& handle);
		void setTexture(std::string const& name, int const& textureUnit, unsigned int const& textureType, unsigned int const& handle);

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
		void clearTexture(std::string const& name, int const& textureUnit, unsigned int const& textureType);

		std::string GetLoadPath() const { return m_loadPath; }
		std::string GetVertexShaderPath() const { return m_loadPath + ".vs"; }
		std::string GetTesslleationControlShaderPath() const { return m_loadPath + ".tcs"; }
		std::string GetTessellationEvaluationShaderPath() const { return m_loadPath + ".tes"; }
		std::string GetGeometryShaderPath() const { return m_loadPath + ".gs"; }
		std::string GetFragmentShaderPath() const { return m_loadPath + ".fs"; }

		unsigned int getSubroutineIndex(unsigned int const& program, std::string const& subroutineName);
		void setSubroutine(unsigned int const& program, unsigned int const& subroutineIndex);

		void setSubPasses(unsigned int const& program, RenderPass* const& renderPasses, int const& numPasses);

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

		unsigned int m_rendererId;
		unsigned int m_shaders[NUM_SHADERS];
		int m_uniforms[NUM_UNIFORMS];
		std::string m_loadPath;
		std::map<std::string, int> m_uniformLookup;
		int m_numPasses;
		RenderPass* m_renderPasses;
	};

}
