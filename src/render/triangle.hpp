
//draw a triangle for test
#pragma once

#include "glheader.hpp"
#include "shader.hpp"

class Application;

class Triangle{
public:
    void init();
    void trick(glm::mat3 &normalMat);
    void free();
    
private:
    // for test
    GLuint vao;
    GLuint vbo;

    Shader shader;
    
    float vertices[3 * 2] = {
        400.0f , 100.0f,
        700.0f , 50.0f,
        800.0f ,  300.0f 
        // -0.5f , -0.5f,
        // 0.5f , -0.5f,
        // 0.0f ,  0.5f 
    };
};

