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
    void init() override;
    void render(GLFWwindow *window) override;
    void onGlfwClose() override;

    void onKey(int key, int scancode, int action, int mods) override;

private:
    std::string mImagePath;
    size_t mVertexes;
    GLuint program;
    GLuint vertexShader;
    GLuint vao;
    GLuint tessControlShader;
    GLuint tessShader;
    GLuint fragmentShader;
    GLuint buffer;
    GLfloat mZoom;
    GLfloat mRotationY;
    GLfloat mRotationX;

    GLuint spawnShader(GLuint shader, const char *shaderPath);
    void calculateVertices(std::vector<GLfloat> &vector, const cv::Mat &mat);
    glm::mat4 calculateTrasformationMatrix();

    void rotationToRange(GLfloat &rotationAngle);
};


#endif //PROJECT_DISPARITYRENDERINGENGINE_HPP
