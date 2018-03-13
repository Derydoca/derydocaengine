#pragma once
#include <map>
#include "Color.h"
#include "glm\glm.hpp"
#include <gl\glew.h>

class Shader;
class Texture;
class RenderTexture;

class Material
{
public:
	Material();
	~Material();

	inline void setShader(Shader* shader) { m_shader = shader; }
	inline Shader* getShader() const { return m_shader; }
	void setTexture(const std::string name, Texture* texture);
	void setTextureSlot(int slot, Texture* texture);
	void setFloat(const std::string name, float value);
	void setVec3(const std::string name, glm::vec3 value);
	void setVec4(const std::string name, glm::vec4 value);
	void setColorRGB(const std::string name, Color value);
	void setColorRGBA(const std::string name, Color value);
	void setMat3(const std::string name, glm::mat3 value);
	void setMat4(const std::string name, glm::mat4 value);
	void setSubroutine(const GLuint program, const GLuint value);
	void bind();
private:
	Shader* m_shader;
	// TODO: Replace this with a BST for multiple textures
	Texture* m_texture;
	std::map<std::string, Texture*> m_textures;
	std::map<std::string, float> m_floatValues;
	std::map<std::string, glm::vec3> m_vec3Values;
	std::map<std::string, glm::vec4> m_vec4Values;
	std::map<std::string, glm::mat3> m_mat3Values;
	std::map<std::string, glm::mat4> m_mat4Values;
	std::map<GLuint, GLuint> m_subroutineValues;
};

