project(derydoca-core)

add_library(${PROJECT_NAME} STATIC
	"./src/Logging/LogMessage.cpp"
	"./src/Logging/Log.cpp" "src/DeviceManager.cpp")

set(PRIVATE_INCLUDES)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)

set(PRIVATE_INCLUDES ${PRIVATE_INCLUDES} "./include/")

if(ENABLE_DX12)
	target_sources(${PROJECT_NAME} PRIVATE "./src/Rendering/DX12/DeviceManagerDX12.cpp")
	target_compile_definitions(${PROJECT_NAME} PUBLIC USE_DX12=1)

	find_package(directx-headers CONFIG REQUIRED)
	set(PRIVATE_INCLUDES ${PRIVATE_INCLUDES} Microsoft::DirectX-Headers Microsoft::DirectX-Guids)
endif()

if(ENABLE_VULKAN)
	target_sources(${PROJECT_NAME} PRIVATE "./src/Rendering/VK/DeviceManagerVK.cpp")
	target_compile_definitions(${PROJECT_NAME} PUBLIC USE_VULKAN=1)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${PRIVATE_INCLUDES})
