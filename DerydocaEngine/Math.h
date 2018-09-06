#pragma once
#include "glm\glm.hpp"

unsigned int MSB = ((unsigned int)-1 >> 1);

bool floatsShareSign(float const& leftValue, float const& rightValue);
glm::vec3 getNormal(glm::vec3 const& vector1, glm::vec3 const& vector2);