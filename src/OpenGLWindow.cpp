//
// Created by dawid on 23.03.17.
//

#include <glad/glad.h>
#include <iostream>
#include "OpenGLWindow.hpp"

OpenGLWindow* OpenGLWindow::mInstance= nullptr;

OpenGLWindow::OpenGLWindow()
{
    mInstance=this;
}

void OpenGLWindow::startWindow(unsigned int width, unsigned int height, const char *windowName) {
    /* Initialize the library */
    if (!glfwInit())
        throw std::runtime_error("glfw could not be initialized");

    /* Create a windowed mode window and its OpenGL context */
    mWindowWidth = width;
    mWindowHeight = height;
    window = glfwCreateWindow(width, height, windowName, NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("could not create opengl context");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        throw std::runtime_error("could not load openGL functions");
    }

    glfwSetKeyCallback(window, onKeyPressed);
    glfwWindowHint(GLFW_REFRESH_RATE, 120);
    init();
    while (!glfwWindowShouldClose(window)) {
        render(window);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}

void OpenGLWindow::onKeyPressed(GLFWwindow *window, int key, int scancode, int action, int mods) {
    mInstance->onKey(key, scancode, action, mods);
}
