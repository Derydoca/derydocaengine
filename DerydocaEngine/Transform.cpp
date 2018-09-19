#include "Transform.h"
#include "GameObject.h"

namespace DerydocaEngine::Components
{

	glm::mat4 Transform::getWorldModel() const
	{
		// Start off with the object's local model
		glm::mat4 worldModel = getModel();

		if (this->m_gameObject != NULL)
		{
			// Temporary storage object for recursive iteration
			DerydocaEngine::GameObject* currentGameObject = this->m_gameObject->getParent();

			// Recursively transform the position by all parent objects
			while (currentGameObject != NULL)
			{
				// Transform the position by the transform's matrix
				worldModel = currentGameObject->getTransform()->getModel() * worldModel;

				// Get the parent transform and start the loop over again
				currentGameObject = currentGameObject->getParent();
			}
		}

		// Return the world position
		return worldModel;
	}

	glm::vec3 Transform::getWorldPos() const
	{
		// Start off with the object's local position
		glm::vec4 pos = glm::vec4(m_pos, 1);

		if (this->m_gameObject != NULL)
		{
			// Temporary storage object for recursive iteration
			DerydocaEngine::GameObject* currentGameObject = this->m_gameObject->getParent();

			// Recursively transform the position by all parent objects
			while (currentGameObject != NULL)
			{
				// Transform the position by the transform's matrix
				pos = currentGameObject->getTransform()->getModel() * pos;

				// Get the parent transform and start the loop over again
				currentGameObject = currentGameObject->getParent();
			}
		}

		// Return the world position
		glm::vec3 worldPos;
		worldPos.x = pos.x;
		worldPos.y = pos.y;
		worldPos.z = pos.z;
		return worldPos;
	}

	void Transform::translate(glm::vec3 const& delta)
	{
		m_pos.x += delta.x;
		m_pos.y += delta.y;
		m_pos.z += delta.z;
	}

}
