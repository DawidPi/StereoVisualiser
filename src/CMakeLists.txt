project(StereoVisualiser)

set(SOURCES
        main.cpp OpenGLWindow.cpp OpenGLWindow.hpp DisparityRenderingEngine.cpp DisparityRenderingEngine.hpp DisparityCalculator.cpp DisparityCalculator.hpp)

find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ../glad/include )
add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} glfw glad glm)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${OPENGL_gl_LIBRARY} ${Boost_LIBRARIES} glfw glad glm)

install(DIRECTORY glsl DESTINATION ${CMAKE_BINARY_DIR})