#include "Transform.h"

void Transform::addChild(Transform* child)
{
	m_children.add(child);
}

Transform::~Transform()
{
}
