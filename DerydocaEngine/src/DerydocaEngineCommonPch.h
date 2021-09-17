#pragma once

// AssImp
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

// Cereal
#include <cereal\cereal.hpp>
#include <cereal\types\polymorphic.hpp>
#include <cereal\archives\json.hpp>
#include <cereal\types\memory.hpp>

// GLEW
#include <GL/glew.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

// Dear IMGUI
#include <vendor/imgui/imgui.h>

// STD Methods
#include <algorithm>
#include <assert.h>
#include <cassert>
#include <functional>
#include <memory>
#include <sys/stat.h>

// STD Objects
#include <atomic>
#include <array>
#include <chrono>
#include <climits>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

// SDL
#include <sdl2/SDL.h>
#undef main

// Derydoca Engine Classes
#include "Logging\Log.h"
#include "Serialization\Serialization.h"
#include "ResourceRef.h"

// Maths
#include "Math/TypeAliases.h"

#if OPENGL||VULKAN
using SystemWindow = SDL_Window;
using GraphicsAPIContext = SDL_GLContext;
#else
using SystemWindow = void;
using GraphicsAPIContext = void;
#endif
