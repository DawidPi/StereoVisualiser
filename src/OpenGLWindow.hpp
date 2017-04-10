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
    virtual void startWindow(const char *windowName);

    virtual void onKey(int key, int scancode, int action, int mods)=0;
    virtual void onScroll(double xOffset, double yOffset)=0;
    virtual void onMouseButton(int button, int action, int mods)=0;
    virtual void onCursorPositionChanged(double xPosition, double yPosition)=0;
    virtual ~OpenGLWindow(){};
    unsigned int getWidth() const {return mWindowWidth;}
    unsigned int getHeight() const {return mWindowHeight;}
    virtual void shutDown();
private:
    GLFWwindow* window;

    static OpenGLWindow* mInstance;
    static void onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void onScrollDone(GLFWwindow *window, double xoffset, double yoffset);
    static void onMouseButtonPressed(GLFWwindow* window, int button, int action, int mods);
    static void onMouseCursorChanged(GLFWwindow* window, double xpos, double ypos);

    unsigned int mWindowWidth;
    unsigned int mWindowHeight;
    bool mShutDown;
};


#endif //PROJECT_OPENGLWINDOW_HPP
