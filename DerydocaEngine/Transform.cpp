#include "Transform.h"
#include "GameObject.h"

glm::vec3 & Transform::getWorldPos()
{
	// Start off with the object's local position
	glm::vec4 pos = glm::vec4(m_pos, 1);

	if (this->m_gameObject != NULL)
	{
		// Temporary storage object for recursive iteration
		GameObject* currentGameObject = this->m_gameObject->getParent();

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
	return glm::vec3(pos);
}

void Transform::translate(glm::vec3 & delta)
{
	m_pos.x += delta.x;
	m_pos.y += delta.y;
	m_pos.z += delta.z;
}
