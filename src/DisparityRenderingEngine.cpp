//
// Created by dawid on 26.03.17.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <opencv2/imgcodecs.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>
#include <glm/gtx/rotate_vector.hpp>

#include "DisparityRenderingEngine.hpp"

DisparityRenderingEngine::DisparityRenderingEngine(std::string imagePath)
        : mImagePath(imagePath),
          mImage()
{
}

DisparityRenderingEngine::DisparityRenderingEngine(cv::Mat &image) :
        mImagePath(),
        mImage(image)

{}

void DisparityRenderingEngine::calculateVertices(std::vector<GLfloat> &vector, const cv::Mat &mat) {
    const cv::Mat image = mat.t();
    GLfloat ratioYX = static_cast<GLfloat>(image.rows) / image.cols;
    const GLfloat glMaxLength = 2.0f;
    const GLfloat maxLength = 1.0f;
    GLfloat lengthX = maxLength;
    GLfloat lengthY = maxLength;
    GLfloat startingY=1 - (glMaxLength - maxLength)/2;
    GLfloat startingX=-1 + (glMaxLength - maxLength)/2;

    if(ratioYX > 1.0f){
        lengthX = lengthY/ratioYX;

        GLfloat updateLength = maxLength - lengthX ;
        startingY -= updateLength/2;
    } else {
        lengthY = lengthX*ratioYX;

        GLfloat updateLength =  maxLength - lengthY;
        startingX += updateLength/2;
    }

    const GLfloat diffX = lengthX/image.cols;
    const GLfloat diffY = lengthY/image.rows;
    for(int row=0; row < image.rows; ++row){
        for(int col=0; col < image.cols; ++col){
            const GLfloat xValue = startingX + row*diffX;
            const GLfloat yValue = startingY - col*diffY;
            const GLfloat depth = image.at<GLfloat>(row,col);
            vector.push_back(xValue);
            vector.push_back(yValue);
            vector.push_back(depth);
        }
    }
}

void DisparityRenderingEngine::init() {
    program = glCreateProgram();
    vertexShader = spawnShader(GL_VERTEX_SHADER, "glsl/VertexShader.glsl");
    tessControlShader = spawnShader(GL_TESS_CONTROL_SHADER, "glsl/TesselationControlShader.glsl");
    tessShader = spawnShader(GL_TESS_EVALUATION_SHADER, "glsl/TesselationShader.glsl");
    fragmentShader = spawnShader(GL_FRAGMENT_SHADER, "glsl/FragmentShader.glsl");

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glCullFace(GL_FRONT_AND_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    cv::Mat image;
    if(!mImagePath.empty())
        image = cv::imread(mImagePath, cv::IMREAD_GRAYSCALE);
    else if(!mImage.empty()){
        image = std::move(mImage);
    } else {
        throw std::runtime_error("invalid disparityMapPath provided or empty disparity image is given.");
    }

    image.convertTo(image, CV_32F, 1.0/255.0);
    std::vector<GLfloat> vertices;
    calculateVertices(vertices, image);
    mVertexes=vertices.size();
    cv::minMaxLoc(image, &mMinimum, &mMaximum);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void DisparityRenderingEngine::render(GLFWwindow *window) {
    glClear(GL_COLOR_BUFFER_BIT);
    const GLfloat depthClear=1.0f;
    glClearBufferfv(GL_DEPTH, 0, &depthClear);
    int width = 0;
    int height = 0;
    glfwGetWindowSize(window, &width, &height);
    glViewport(0,0,width, height);
    glUseProgram(program);
    auto wholeTransformationMatrix = calculateTrasformationMatrix();
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(wholeTransformationMatrix));
    glUniform1f(2, static_cast<GLfloat>(mMinimum));
    glUniform1f(3, static_cast<GLfloat>(mMaximum));
    glEnableVertexAttribArray(0);
    glUniform1i(1, mColor);
    glDrawArrays(GL_POINTS, 0, (GLsizei) mVertexes);
}

void DisparityRenderingEngine::onGlfwClose() {
    glDeleteVertexArrays(1,&vao);
    glDeleteProgram(program);
    glDeleteBuffers(1, &buffer);
}

GLuint DisparityRenderingEngine::spawnShader(GLuint shader, const char *shaderPath) {
    auto createdShader = glCreateShader(shader);
    std::string stream;
    std::ifstream createdShaderFile;
    createdShaderFile.open(shaderPath);
    stream = std::string((std::istreambuf_iterator<char>(createdShaderFile)), std::istreambuf_iterator<char>());
    const char* rawSource = stream.c_str();
    glShaderSource(createdShader, 1, &rawSource, nullptr);
    glCompileShader(createdShader);

    GLint success;
    glGetShaderiv(createdShader, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE){
        GLint log_size=0;
        glGetShaderiv(createdShader, GL_INFO_LOG_LENGTH, &log_size);
        std::vector<GLchar> errorLog((unsigned long) log_size);
        glGetShaderInfoLog(createdShader, (GLsizei) errorLog.size(), nullptr, errorLog.data());

        glDeleteShader(createdShader);
        createdShaderFile.close();
        throw std::runtime_error(&errorLog[0]);
    }
    createdShaderFile.close();
    return createdShader;
}

glm::mat4 DisparityRenderingEngine::calculateTrasformationMatrix() {
    auto frustum =  glm::perspective(-50.0f, static_cast<float>(getWidth())/getHeight(), 1.0f, 1000.0f ); //glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 2.0f);
    auto rotationXYZ = glm::mat4(1.0f);
    rotationXYZ = glm::rotate(rotationXYZ, mRotationX, glm::vec3(1.0f,0.0f,0.0f));
    rotationXYZ = glm::rotate(rotationXYZ, mRotationY, glm::vec3(0.0f,1.0f,0.0f));
    rotationXYZ = glm::rotate(rotationXYZ, 0.0f, glm::vec3(0.0f,0.0f,1.0f));
    auto translation = glm::mat4(1.0f);
    auto translationBase = -7.0f;
    translation = glm::translate(translation, glm::vec3(0.0f, 0.0f, translationBase + mZoom));
    auto centerTranslation = glm::mat4(1.0f);
    return  frustum * translation * rotationXYZ;
}

void DisparityRenderingEngine::onKey(int key, int scancode, int action, int mods) {
    const GLfloat rotationDiff = glm::pi<float>()/500;
    const GLfloat zoomDiff = 0.05;
    if(key == ']'){
        mZoom += zoomDiff;
    }
    if(key == '['){
        mZoom -= zoomDiff;
    }
    if(key == 'W'){
        mRotationX += rotationDiff;
        rotationToRange(mRotationX);
    }
    if(key == 'S'){
        mRotationX -= rotationDiff;
        rotationToRange(mRotationX);
    }
    if(key == 'A'){
        mRotationY += rotationDiff;
        rotationToRange(mRotationY);
    }
    if(key == 'D'){
        mRotationY -= rotationDiff;
        rotationToRange(mRotationY);
    }
    if(key == 'Z'){
        mColor = GL_TRUE;
    }
    if(key == 'X'){
        mColor = GL_FALSE;
    }
    if(key == 'C'){
        mRotationX=0;
        mRotationY=0;
        mZoom=0;
    }
    const auto escapeButton = 256;
    if(key == escapeButton){
        shutDown();
    }
}

void DisparityRenderingEngine::rotationToRange(GLfloat &rotationAngle) {
    if(rotationAngle > glm::pi<GLfloat>()){
        rotationAngle = glm::pi<GLfloat>();
    } else if(rotationAngle < -glm::pi<GLfloat>()){
        rotationAngle = -glm::pi<GLfloat>();
    }
}

void DisparityRenderingEngine::onScroll(double xOffset, double yOffset) {
    mZoom+=0.5f*yOffset;
}

void DisparityRenderingEngine::onMouseButton(int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT){
        if(action == GLFW_PRESS)
            mMouseButtonPressed=true;
        else if(action == GLFW_RELEASE)
            mMouseButtonPressed=false;
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT){
        if(action == GLFW_PRESS)
            mColor = (GLboolean) !mColor;
    }
}

void DisparityRenderingEngine::onCursorPositionChanged(double xPosition, double yPosition) {
    MousePosition currentMousePosition(xPosition, yPosition);

    if(mMouseButtonPressed){
        const double scale=0.005;
        mRotationY -= scale*(mMouseCursorPosition.xPosition - currentMousePosition.xPosition);
        rotationToRange(mRotationX);
        mRotationX -= scale*(mMouseCursorPosition.yPosition - currentMousePosition.yPosition);
        rotationToRange(mRotationY);
    }

    mMouseCursorPosition = currentMousePosition;
}

