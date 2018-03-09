#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

class GameObject;

struct Transform
{
public:

	Transform() :
		m_pos(glm::vec3()),
		m_quat(glm::fquat()),
		m_scale(glm::vec3(1, 1, 1))
	{
	}

	Transform(const glm::vec3& pos) :
		m_pos(pos),
		m_quat(glm::fquat()),
		m_scale(glm::vec3(1,1,1))
	{
	}

	Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) :
		m_pos(pos),
		m_scale(scale)
	{
		setEulerAngles(rot);
	}

	Transform(const glm::vec3& pos, const glm::fquat& quat, const glm::vec3& scale) :
		m_pos(pos),
		m_quat(quat),
		m_scale(scale)
	{
	}

	~Transform() {};

	inline glm::mat4 getModel() const { return glm::translate(m_pos) * glm::mat4_cast(m_quat) * glm::scale(m_scale); }
	inline glm::mat4 getTranslationMatrix() const { return glm::translate(m_pos); }
	inline glm::mat4 getRotationMatrix() const { return glm::mat4_cast(m_quat); }
	inline glm::mat4 getScaleMatrix() const { return glm::scale(m_scale); }
	glm::mat4 getWorldModel();

	glm::vec3 getWorldPos();
	inline glm::vec3& getPos() { return m_pos; }
	inline glm::fquat& getQuat() { return m_quat; }
	inline glm::vec3& getScale() { return m_scale; }

	inline void setPos(const glm::vec3& pos) { m_pos = pos; }
	inline void setEulerAngles(const glm::vec3& euler) { m_quat = glm::fquat(euler * 0.0174533f); }
	inline void setQuat(const glm::fquat& quat) { m_quat = quat; }
	inline void setScale(const glm::vec3& scale) { m_scale = scale; }
	void setGameObject(GameObject* object) { m_gameObject = object; }

	void translate(glm::vec3& delta);
private:
	glm::vec3 m_pos;
	glm::vec3 m_scale;
	glm::fquat m_quat;
	GameObject* m_gameObject;
};
