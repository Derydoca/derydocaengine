#pragma once
#include <map>
#include <glm\mat3x3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <memory>
#include <vector>

namespace DerydocaEngine
{
	struct Color;
	namespace Rendering {
		class Shader;
		class Texture;
	}
}
class RenderTexture;

namespace DerydocaEngine::Rendering
{

	class Material
	{
	public:
		Material();
		~Material();

		inline void setShader(std::shared_ptr<Shader> shader) { m_shader = shader; }
		inline std::shared_ptr<Rendering::Shader> getShader() const { return m_shader; }
		
		void bind() const;
		void copyFrom(std::shared_ptr<Material> other);
		void unbind();

		void setBool(const std::string& name, bool const& value);
		void setColorRGB(const std::string& name, Color const& value);
		void setColorRGBA(const std::string& name, Color const& value);
		void setFloat(const std::string& name, float const& value);
		void setInt(const std::string& name, int const& value);
		void setMat3(const std::string& name, glm::mat3 const& value);
		void setMat4(const std::string& name, glm::mat4 const& value);
		void setMat4Array(const std::string& name, std::vector<glm::mat4> matrixArray);
		void setSubroutine(unsigned int program, unsigned int value);
		void setTexture(const std::string& name, std::shared_ptr<Texture> texture);
		void setTextureSlot(int const& slot, std::shared_ptr<Texture> texture);
		void setVec3(const std::string& name, glm::vec3 const& value);
		void setVec4(const std::string& name, glm::vec4 const& value);

		bool boolExists(const std::string& name);
		bool colorRGBExists(const std::string& name);
		bool colorRGBAExists(const std::string& name);
		bool floatExists(const std::string& name);
		bool intExists(const std::string& name);
		bool mat3Exists(const std::string& name);
		bool mat4Exists(const std::string& name);
		bool mat4ArrayExists(const std::string& name);
		bool subroutineValueExists(unsigned int program);
		bool textureExists(const std::string& name);
		bool textureSlotExists(int slot);
		bool vec3Exists(const std::string& name);
		bool vec4Exists(const std::string& name);
		
		bool getBool(const std::string& name);
		Color getColorRGB(const std::string& name);
		Color getColorRGBA(const std::string& name);
		float getFloat(const std::string& name);
		int getInt(const std::string& name);
		glm::mat3 getMat3(const std::string& name);
		glm::mat4 getMat4(const std::string& name);
		std::vector<glm::mat4> getMat4Array(const std::string& name);
		unsigned int getSubroutineValue(unsigned int program);
		std::shared_ptr<Texture> getTexture(const std::string& name);
		std::shared_ptr<Texture> getTextureSlot(int slot);
		glm::vec3 getVec3(const std::string& name);
		glm::vec4 getVec4(const std::string& name);
		
	private:
		std::shared_ptr<Shader> m_shader;
		// TODO: Replace this with a BST for multiple textures
		std::shared_ptr<Texture> m_texture;
		std::map<std::string, bool> m_boolValues;
		std::map<std::string, int> m_intValues;
		std::map<std::string, std::shared_ptr<Texture>> m_textures;
		std::map<std::string, float> m_floatValues;
		std::map<std::string, glm::vec3> m_vec3Values;
		std::map<std::string, glm::vec4> m_vec4Values;
		std::map<std::string, glm::mat3> m_mat3Values;
		std::map<std::string, glm::mat4> m_mat4Values;
		std::map<std::string, std::vector<glm::mat4>> m_mat4ArrayValues;
		std::map<unsigned int, unsigned int> m_subroutineValues;
	};

}
