#pragma once
#include "glm\glm.hpp"

#if _WIN64
int MSB = 0x8000000000000000;
#elif _WIN32
int MSB = 0x80000000;
#endif

bool floatsShareSign(float leftValue, float rightValue);
glm::vec3 getNormal(glm::vec3 vector1, glm::vec3 vector2);