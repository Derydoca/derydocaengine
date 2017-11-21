#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

// TODO: Define the methods in the cpp file
class Transform
{
public:

	Transform() :
		m_pos(glm::vec3(0, 0, 0)),
		m_quat(glm::tquat<float>(0, 0, 0, 1)),
		m_scale(glm::vec3(1, 1, 1))
	{
	}

	Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) :
		m_pos(pos),
		m_scale(scale)
	{
		SetEulerAngles(rot);
	}

	Transform(const glm::vec3& pos, const glm::tquat<float>& quat, const glm::vec3& scale) :
		m_pos(pos),
		m_quat(quat),
		m_scale(scale)
	{
	}

	inline glm::mat4 GetModel() const {
		glm::mat4 matrix = glm::translate(m_pos) * glm::mat4_cast(m_quat) * glm::scale(m_scale);
		return matrix;
	}

	inline glm::vec3& GetPos() { return m_pos; }
	inline glm::vec3& GetScale() { return m_scale; }

	inline void SetPos(const glm::vec3& pos) { m_pos = pos; }
	inline void SetEulerAngles(const glm::vec3& euler) { m_quat = glm::tquat<float>(euler); }
	inline void SetScale(const glm::vec3& scale) { m_scale = scale; }

	~Transform();
private:
	glm::vec3 m_pos;
	glm::vec3 m_scale;

	glm::tquat<float> m_quat;
};
