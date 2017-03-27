cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 14)


add_subdirectory(glm)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(src)

