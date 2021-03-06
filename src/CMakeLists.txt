project(StereoVisualiser)

set(SOURCES
        main.cpp
        OpenGLWindow.cpp
        OpenGLWindow.hpp
        DisparityRenderingEngine.cpp
        DisparityRenderingEngine.hpp
        SGBMDisparityCalculator.cpp
        SGBMDisparityCalculator.hpp
        DisparityCalculator.hpp
        SADDisparityCalculator.cpp
        SADDisparityCalculator.hpp
        )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(SYSTEM ${OPENGL_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ../glad/include ../glfw/include )
add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} glfw glad glm)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${OPENGL_gl_LIBRARY} ${Boost_LIBRARIES} glfw glad glm)

install(DIRECTORY glsl DESTINATION ${CMAKE_BINARY_DIR})