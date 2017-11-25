#pragma once

#include <glm/glm.hpp>

struct Vertex
{
public:
	Vertex() :
		m_pos(glm::vec3()),
		m_texCoord(glm::vec3()),
		m_normal(glm::vec3(0, 1, 0))
	{
	}

	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal = glm::vec3(0, 0, 0)) :
		m_pos(pos),
		m_texCoord(texCoord),
		m_normal(normal)
	{
	}

	inline glm::vec3* getPos() { return &m_pos; }
	inline glm::vec2* getTexCoord() { return &m_texCoord; }
	inline glm::vec3* getNormal() { return &m_normal; }

	inline void setPos(glm::vec3 pos) { m_pos = pos; }
	inline void setPos(float x, float y, float z) { m_pos = glm::vec3(x, y, z); }
	inline void setTexCoord(glm::vec2 texCoord) { m_texCoord = texCoord; }
	inline void setTexCoord(float u, float v) { m_texCoord = glm::vec2(u, v); }
	inline void setNormal(glm::vec3 normal) { m_normal = normal; }
	inline void setNormal(float x, float y, float z) { m_normal = glm::vec3(x, y, z); }

protected:
private:
	glm::vec3 m_pos;
	glm::vec2 m_texCoord;
	glm::vec3 m_normal;
};