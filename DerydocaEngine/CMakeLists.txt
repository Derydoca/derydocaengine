project(derydoca-engine-core)

find_package(spdlog CONFIG REQUIRED)

add_library(${PROJECT_NAME} STATIC
	"./src/Logging/LogMessage.cpp"
	"./src/Logging/Log.cpp"
	"src/Logging/LogLevel.h"
	"src/Logging/LogDomain.h")

target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)

target_include_directories(${PROJECT_NAME} PUBLIC "./src/")
