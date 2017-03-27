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
    virtual void onKey(int key, int scancode, int action, int mods)=0;
    virtual ~OpenGLWindow(){};
    unsigned int getWidth() const {return mWindowWidth;}
    unsigned int getHeight() const {return mWindowHeight;}
private:
    GLFWwindow* window;

    static OpenGLWindow* mInstance;
    static void onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);

    unsigned int mWindowWidth;
    unsigned int mWindowHeight;
};


#endif //PROJECT_OPENGLWINDOW_HPP
