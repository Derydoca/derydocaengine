#pragma once
#include "glm\glm.hpp"

unsigned int MSB = ((unsigned int)-1 >> 1);

bool floatsShareSign(float leftValue, float rightValue);
glm::vec3 getNormal(glm::vec3 vector1, glm::vec3 vector2);