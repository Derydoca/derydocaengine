#include "Math.h"
#include <cmath>

bool floatsShareSign(float const& leftValue, float const& rightValue)
{
	// Compare the sign of each float (MSB of binary value)
	return signbit(leftValue) == signbit(rightValue);
}

glm::vec3 getNormal(glm::vec3 const& vector1, glm::vec3 const& vector2)
{
	// The cross product of the two vectors returns a normal perpendicular to the plane defined by the vectors
	return glm::cross(vector1, vector2);
}

