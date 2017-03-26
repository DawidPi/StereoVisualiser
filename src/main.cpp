#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include "OpenGLWindow.hpp"

class Window : public OpenGLWindow{
public:

    Window(std::string imagePath)
        : mImagePath(imagePath)
    {
    }

    void calculateVertices(std::vector<GLfloat> &vector, const cv::Mat &mat) {
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

    void init() override {
        program = glCreateProgram();
        vertexShader = spawnShader(GL_VERTEX_SHADER, "glsl/VertexShader.glsl");
        tessControlShader = spawnShader(GL_TESS_CONTROL_SHADER, "glsl/TesselationControlShader.glsl");
        tessShader = spawnShader(GL_TESS_EVALUATION_SHADER, "glsl/TesselationShader.glsl");
        fragmentShader = spawnShader(GL_FRAGMENT_SHADER, "glsl/FragmentShader.glsl");

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        //glAttachShader(program, tessControlShader);
        //glAttachShader(program, tessShader);
        glLinkProgram(program);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1,&buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        cv::Mat image = cv::imread(mImagePath, cv::IMREAD_GRAYSCALE);
        image.convertTo(image, CV_32F, 1.0/255.0);
        std::vector<GLfloat> vertices;
        calculateVertices(vertices, image);
        mVertexes=vertices.size();
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    }

    void render(GLFWwindow *window) override {
        glClear(GL_COLOR_BUFFER_BIT);
        int width = 0;
        int height = 0;
        glfwGetWindowSize(window, &width, &height);
        glViewport(0,0,width, height);
        glUseProgram(program);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_POINTS, 0, (GLsizei) mVertexes);
    }

    void onGlfwClose() override {
        glDeleteVertexArrays(1,&vao);
        glDeleteProgram(program);
        glDeleteBuffers(1, &buffer);
    }
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

    GLuint spawnShader(GLuint shader, const char *shaderPath) {
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
};

int main(int argc, char* argv[])
{
    if(argc != 2){
        std::cerr << "No image given as argument";
        return 0;
    }
    std::string imagePath(argv[1]);

    Window window(imagePath);

    window.startWindow(500,500, "StereoVisualiser");
    return 0;
}