cmake_minimum_required (VERSION 3.8)
include(FetchContent)

project(software_renderer)
include_directories("src/headers/")

add_executable (software_renderer "src/main.cpp"
				"src/headers/model.h" "src/src/model.cpp"
				"src/headers/tgaimage.h" "src/src/tgaimage.cpp" 
				"src/headers/drawer.h" "src/src/drawer.cpp")

add_subdirectory("libs/glm")
add_subdirectory("libs/glfw-3.4")

target_link_libraries(software_renderer PRIVATE glm::glm)
target_link_libraries(software_renderer PRIVATE glfw)
