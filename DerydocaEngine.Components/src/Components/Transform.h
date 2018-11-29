#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

namespace DerydocaEngine {
	class GameObject;
}

namespace DerydocaEngine::Components
{

	struct Transform
	{
	public:

		Transform() :
			m_pos(glm::vec3()),
			m_quat(glm::fquat()),
			m_scale(glm::vec3(1, 1, 1))
		{
		}

		Transform(glm::vec3 const& pos) :
			m_pos(pos),
			m_quat(glm::fquat()),
			m_scale(glm::vec3(1, 1, 1))
		{
		}

		Transform(glm::vec3 const& pos, glm::vec3 const& rot, glm::vec3 const& scale) :
			m_pos(pos),
			m_scale(scale)
		{
			setEulerAngles(rot);
		}

		Transform(glm::vec3 const& pos, glm::fquat const& quat, glm::vec3 const& scale) :
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
		glm::mat4 getWorldModel() const;

		glm::vec3 getWorldPos() const;
		inline glm::vec3 getPos() const { return m_pos; }
		inline glm::fquat getQuat() const { return m_quat; }
		inline glm::vec3 getScale() const { return m_scale; }
		GameObject* getGameObject() const { return m_gameObject; }

		inline void setPos(glm::vec3 const& pos) { m_pos = pos; }
		inline void setEulerAngles(glm::vec3 const& euler) { m_quat = glm::fquat(euler * 0.0174533f); }
		inline void setQuat(glm::fquat const& quat) { m_quat = quat; }
		inline void setScale(glm::vec3 const& scale) { m_scale = scale; }
		void setGameObject(GameObject* object) { m_gameObject = object; }

		void translate(glm::vec3 const& delta);
	private:
		glm::vec3 m_pos;
		glm::vec3 m_scale;
		glm::fquat m_quat;
		GameObject* m_gameObject;
	};

}
