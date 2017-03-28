//
// Created by dawid on 26.03.17.
//

#ifndef PROJECT_DISPARITYRENDERINGENGINE_HPP
#define PROJECT_DISPARITYRENDERINGENGINE_HPP


#include <opencv2/core/mat.hpp>
#include "OpenGLWindow.hpp"
#include <glm/mat4x4.hpp>

class DisparityRenderingEngine : public OpenGLWindow{
public:

    DisparityRenderingEngine(std::string imagePath);
    DisparityRenderingEngine(cv::Mat& image);
    void init() override;
    void render(GLFWwindow *window) override;
    void onGlfwClose() override;

private:
    std::string mImagePath;
    cv::Mat mImage;

    size_t mVertexes;
    GLuint program;
    GLuint vertexShader;
    GLuint vao;
    GLuint tessControlShader;
    GLuint tessShader;
    GLuint fragmentShader;
    GLuint buffer;
    GLfloat mZoom = 0.0f;
    GLfloat mRotationY = 0.0f;
    GLfloat mRotationX = 0.0f;
    GLboolean mColor = GL_TRUE;
    GLdouble mMinimum;
    GLdouble mMaximum;
    bool mMouseButtonPressed = false;

private:
    void rotationToRange(GLfloat &rotationAngle);
    GLuint spawnShader(GLuint shader, const char *shaderPath);
    void calculateVertices(std::vector<GLfloat> &vector, const cv::Mat &mat);
    glm::mat4 calculateTrasformationMatrix();

private:
    struct MousePosition{
        MousePosition(double xPos, double yPos) : xPosition(xPos), yPosition(yPos){}
        double xPosition;
        double yPosition;
    } mMouseCursorPosition = MousePosition(0,0);

    void onCursorPositionChanged(double xPosition, double yPosition) override;
    void onKey(int key, int scancode, int action, int mods) override;
    void onScroll(double xOffset, double yOffset) override;
    void onMouseButton(int button, int action, int mods) override;
};


#endif //PROJECT_DISPARITYRENDERINGENGINE_HPP
