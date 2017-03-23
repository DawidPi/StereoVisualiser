project(glad)

include_directories(include)
set(SOURCES src/glad.c include/glad/glad.h include/KHR/khrplatform.h)

add_library(${PROJECT_NAME} ${SOURCES})