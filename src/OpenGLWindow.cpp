//
// Created by dawid on 23.03.17.
//

#include <glad/glad.h>
#include <iostream>
#include "OpenGLWindow.hpp"

OpenGLWindow* OpenGLWindow::mInstance= nullptr;

OpenGLWindow::OpenGLWindow() : mShutDown(false)
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
    auto primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    window = glfwCreateWindow(mode->width, mode->height, windowName, primaryMonitor, NULL);
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
    glfwSetScrollCallback(window, onScrollDone);
    glfwSetMouseButtonCallback(window, onMouseButtonPressed);
    glfwSetCursorPosCallback(window, onMouseCursorChanged);
    init();
    while (!glfwWindowShouldClose(window)) {
        if(mShutDown){
            glfwDestroyWindow(window);
            glfwTerminate();
            return;
        }

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

void OpenGLWindow::onMouseButtonPressed(GLFWwindow *window, int button, int action, int mods) {
    mInstance->onMouseButton(button, action, mods);
}

void OpenGLWindow::onScrollDone(GLFWwindow *window, double xoffset, double yoffset) {
    mInstance->onScroll(xoffset, yoffset);
}

void OpenGLWindow::onMouseCursorChanged(GLFWwindow *window, double xpos, double ypos) {
    mInstance->onCursorPositionChanged(xpos, ypos);
}

void OpenGLWindow::shutDown(){
    mShutDown = true;
}

