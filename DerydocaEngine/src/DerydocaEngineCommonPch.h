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
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

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
#include <glm/gtx/transform.hpp>

// YAML-CPP
#include <yaml-cpp/yaml.h>

// STD Methods
#include <algorithm>
#include <assert.h>
#include <cassert>
#include <functional>
#include <memory>
#include <sys/stat.h>

// STD Objects
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