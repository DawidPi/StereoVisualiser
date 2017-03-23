#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "OpenGLWindow.hpp"

class Window : public OpenGLWindow{
public:

    void init() override {
        program = glCreateProgram();
        vertexShader = spawnShader(GL_VERTEX_SHADER, "glsl/VertexShader.glsl");
        tessControlShader = spawnShader(GL_TESS_CONTROL_SHADER, "glsl/TesselationControlShader.glsl");
        tessShader = spawnShader(GL_TESS_EVALUATION_SHADER, "glsl/TesselationShader.glsl");
        fragmentShader = spawnShader(GL_FRAGMENT_SHADER, "glsl/FragmentShader.glsl");

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glAttachShader(program, tessControlShader);
        glAttachShader(program, tessShader);
        glLinkProgram(program);
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1,&buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        GLfloat vertexPos[]{
                -0.25f, 0.25f, -0.25f, -0.25f, -0.25f, -0.25f, 0.25f, -0.25f, -0.25f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPos), vertexPos, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    }

    void render() override {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_PATCHES, 0, 3);
    }

    void onGlfwClose() override {
        glDeleteVertexArrays(1,&vao);
        glDeleteProgram(program);
        glDeleteBuffers(1, &buffer);
    }
private:
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

int main(void)
{
    Window window;
    window.startWindow(500,500, "StereoVisualiser");
    return 0;
}