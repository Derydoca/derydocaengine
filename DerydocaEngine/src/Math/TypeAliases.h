#pragma once
#include "glm/vec2.hpp"

using int2 = glm::ivec2;
using float2 = glm::vec2;
using float3 = glm::vec3;
using float4 = glm::vec4;

// Move this somewhere else
template<class T>
using sptr = std::shared_ptr<T>;