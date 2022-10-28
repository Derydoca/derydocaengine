project(derydoca-core)

add_library(${PROJECT_NAME} STATIC
	"./src/Logging/LogMessage.cpp"
	"./src/Logging/Log.cpp" "src/DeviceManager.cpp")

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)

target_include_directories(${PROJECT_NAME} PRIVATE "./src/" "./include/")

if(ENABLE_DX12)
	target_sources(${PROJECT_NAME} PRIVATE "./src/Rendering/DX12/DeviceManagerDX12.cpp")
	target_compile_definitions(${PROJECT_NAME} PUBLIC USE_DX12=1)
endif()

if(ENABLE_VULKAN)
	target_sources(${PROJECT_NAME} PRIVATE "./src/Rendering/VK/DeviceManagerVK.cpp")
	target_compile_definitions(${PROJECT_NAME} PUBLIC USE_VULKAN=1)
endif()
