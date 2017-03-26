//
// Created by dawid on 23.03.17.
//

#ifndef PROJECT_OPENGLWINDOW_HPP
#define PROJECT_OPENGLWINDOW_HPP

#include <GLFW/glfw3.h>

class OpenGLWindow {
public:
    OpenGLWindow();
    OpenGLWindow(const OpenGLWindow& rhs)= delete;
    OpenGLWindow& operator=(const OpenGLWindow& rhs)= delete;

    virtual void init()=0;
    virtual void render(GLFWwindow *window)=0;
    virtual void startWindow(unsigned int width, unsigned int length, const char *windowName = "");
    virtual void onGlfwClose()=0;
    virtual ~OpenGLWindow(){};
private:
    GLFWwindow* window;
};


#endif //PROJECT_OPENGLWINDOW_HPP
