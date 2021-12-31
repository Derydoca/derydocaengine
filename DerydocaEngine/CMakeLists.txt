project(derydoca-engine-core)

add_library(${PROJECT_NAME} STATIC
	"./src/Logging/LogMessage.cpp"
	"./src/Logging/Log.cpp" "src/DeviceManager.cpp")

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

target_include_directories(${PROJECT_NAME} PRIVATE "./src/" "./include/")
target_include_directories(${PROJECT_NAME} PUBLIC "../vendor/nvrhi/include/")

if(ENABLE_DX12)
	target_sources(${PROJECT_NAME} PRIVATE "./src/Rendering/DX12/DeviceManagerDX12.cpp")
	target_compile_definitions(${PROJECT_NAME} PUBLIC USE_DX12=1)
	target_link_libraries(${PROJECT_NAME} PUBLIC nvrhi_d3d12)
endif()

if(ENABLE_VULKAN)
	target_sources(${PROJECT_NAME} PRIVATE "./src/Rendering/VK/DeviceManagerVK.cpp")
	target_compile_definitions(${PROJECT_NAME} PUBLIC USE_VULKAN=1)
	target_link_libraries(${PROJECT_NAME} PUBLIC nvrhi_vk)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE nvrhi)
