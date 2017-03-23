project(StereoVisualiser)

set(SOURCES
        main.cpp OpenGLWindow.cpp OpenGLWindow.hpp)

find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS} ../glad/include)
add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} glfw glad)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${OPENGL_gl_LIBRARY} glfw glad)

install(DIRECTORY glsl DESTINATION ${CMAKE_BINARY_DIR})