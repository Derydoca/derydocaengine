#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include "Object.h"

namespace DerydocaEngine {
	class GameObject;
}

namespace DerydocaEngine::Components
{

	struct Transform : public Object
	{
	public:
		REGISTER_TYPE_ID(Transform);

		Transform() :
			m_Position(glm::vec3()),
			m_Scale(glm::vec3(1, 1, 1)),
			m_Quaternion(glm::fquat()),
			m_GameObject()
		{
		}

		Transform(glm::vec3 const& pos) :
			m_Position(pos),
			m_Scale(glm::vec3(1, 1, 1)),
			m_Quaternion(glm::fquat()),
			m_GameObject()
		{
		}

		Transform(glm::vec3 const& pos, glm::vec3 const& rot, glm::vec3 const& scale) :
			m_Position(pos),
			m_Scale(scale),
			m_Quaternion(),
			m_GameObject()
		{
			setEulerAngles(rot);
		}

		Transform(glm::vec3 const& pos, glm::fquat const& quat, glm::vec3 const& scale) :
			m_Position(pos),
			m_Quaternion(quat),
			m_Scale(scale),
			m_GameObject()
		{
		}

		~Transform() {};

		inline glm::mat4 getModel() const { return glm::translate(m_Position) * glm::mat4_cast(m_Quaternion) * glm::scale(m_Scale); }
		inline glm::mat4 getTranslationMatrix() const { return glm::translate(m_Position); }
		inline glm::mat4 getRotationMatrix() const { return glm::mat4_cast(m_Quaternion); }
		inline glm::mat4 getScaleMatrix() const { return glm::scale(m_Scale); }
		glm::mat4 getWorldModel() const;

		glm::vec3 getWorldPosition() const;
		inline glm::vec3& getPosition() { return m_Position; }
		inline glm::vec3 getPosition() const { return m_Position; }
		inline glm::fquat& getQuaternion() { return m_Quaternion; }
		inline glm::fquat getQuaternion() const { return m_Quaternion; }
		inline glm::vec3& getScale() { return m_Scale; }
		inline glm::vec3 getScale() const { return m_Scale; }
		GameObject* getGameObject() const { return m_GameObject; }

		inline void setPosition(glm::vec3 const& pos) { m_Position = pos; }
		inline void setEulerAngles(glm::vec3 const& euler) { m_Quaternion = glm::fquat(euler * 0.0174533f); }
		inline void setQuaternion(glm::fquat const& quat) { m_Quaternion = quat; }
		inline void setScale(glm::vec3 const& scale) { m_Scale = scale; }
		void setGameObject(GameObject* object) { m_GameObject = object; }

		void translate(glm::vec3 const& delta);

		SERIALIZE_FUNC(
			SERIALIZE(m_Position),
			SERIALIZE(m_Scale),
			SERIALIZE(m_Quaternion)
		)
	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::fquat m_Quaternion;
		GameObject* m_GameObject;
	};

}
