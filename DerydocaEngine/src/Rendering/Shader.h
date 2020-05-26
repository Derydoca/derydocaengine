#pragma once
#include <map>
#include <memory>
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
		Shader(const std::string& fileName);
		Shader(const std::string& fileName, const int count, const char *const * varyings);
		~Shader();

		void bind();
		void update(
			const std::shared_ptr<MatrixStack>& matrixStack,
			const Projection& projection,
			const std::shared_ptr<Components::Transform> trans
		);
		void update(const glm::mat4& matrix);
		void updateViaActiveCamera(const std::shared_ptr<MatrixStack>& matrixStack);
		bool isValid() { return m_rendererId != 0; }

		void bindUniformBuffer(const std::string& name, int uniformBufferId);
		void setFloat(const std::string& name, const float val);
		void setFloatArray(const std::string& name, const std::vector<float>& value);
		void setFloatArray(const std::string& name, const float* arrayLocation, const unsigned int arrayLength);
		void setColorRGB(const std::string& name, const Color& color);
		void setColorRGBA(const std::string& name, const Color& color);
		void setInt(const std::string& name, const int val);
		void setIntArray(const std::string& name, const int* arrayLocation, const unsigned int arrayLength);
		void setVec3(const std::string& name, const glm::vec3& val);
		void setVec4(const std::string& name, const glm::vec4& val);
		void setMat3(const std::string& name, const glm::mat3& val);
		void setMat4(const std::string& name, const glm::mat4& val);
		void setMat4Array(const std::string& name, const std::vector<glm::mat4>& valArray);
		void setTexture(const std::string& name, const int textureUnit, const std::shared_ptr<Rendering::Texture> handle);
		void setTexture(const std::string& name, const int textureUnit, const unsigned int textureType, const unsigned int handle);

		void clearFloat(const std::string& name);
		void clearFloatArray(const std::string& name, const unsigned int arrayLength);
		void clearColorRGB(const std::string& name);
		void clearColorRGBA(const std::string& name);
		void clearInt(const std::string& name);
		void clearIntArray(const std::string& name, const unsigned int arrayLength);
		void clearVec3(const std::string& name);
		void clearVec4(const std::string& name);
		void clearMat3(const std::string& name);
		void clearMat4(const std::string& name);
		void clearTexture(const std::string& name, const int textureUnit, const unsigned int textureType);

		std::string GetLoadPath() const { return m_loadPath; }
		std::string GetVertexShaderPath() const { return m_loadPath + ".vs"; }
		std::string GetTesslleationControlShaderPath() const { return m_loadPath + ".tcs"; }
		std::string GetTessellationEvaluationShaderPath() const { return m_loadPath + ".tes"; }
		std::string GetGeometryShaderPath() const { return m_loadPath + ".gs"; }
		std::string GetFragmentShaderPath() const { return m_loadPath + ".fs"; }
		unsigned int getRendererId() const { return m_rendererId; }

		unsigned int getSubroutineIndex(const unsigned int program, const std::string& subroutineName);
		void setSubroutine(const unsigned int program, const unsigned int subroutineIndex);

		void setSubPasses(const unsigned int program, RenderPass* renderPasses, const int numPasses);

		void renderMesh(const std::shared_ptr<Mesh> mesh, const std::shared_ptr<RenderTexture> renderTexture);
	private:
		static const unsigned int NUM_SHADERS = 5;
		Shader(const Shader& other) {}
		void operator=(const Shader& other) {}
		int getUniformLocation(const std::string& stringName);
		void setTransformFeedbackVaryings(const int count, const char *const * varyings);
		void deleteShaderProgram();

		static bool CheckShaderError(const unsigned int shader, const unsigned int flag, const bool isProgram, const std::string& errorMessage);
		static std::string LoadShader(const std::string& fileName);
		static bool CheckIfShaderExists(const std::string& fileName);
		static unsigned int CreateShader(const std::string& text, const unsigned int shaderType);
		static int getErrorShaderID();

		enum {
			TRANSFORM_MVP = 0,
			TRANSFORM_MV = 1,
			TRANSFORM_NORMAL = 2,
			TRANSFORM_PROJECTION = 3,
			TRANSFORM_MODEL = 4,
			TRANSFORM_VIEW = 5,

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
