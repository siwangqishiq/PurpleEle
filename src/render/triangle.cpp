#include "triangle.hpp"
#include "log.hpp"

void Triangle::init(){
    shader = ShaderManager::getInstance()->loadAssetShader("triangle_render"
                , "shader/triangle_vert.glsl" 
                , "shader/triangle_frag.glsl");
    
    glGenVertexArrays(1 , &vao);
    glGenBuffers(1 , &vbo);
    
    Logi("test" , "vao = %d",vao);
    Logi("test" , "vbo = %d",vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER , vbo);
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER , 3 *2 * sizeof(float) , vertices , GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0 , 2 , GL_FLOAT , GL_FALSE , 2 * sizeof(float) , (void *)0);
}

void Triangle::trick(glm::mat3 &normalMat){
    shader.useShader();
    shader.setUniformMat3("transMat" , normalMat);

    // glBindVertexArray(vao);
    // // Logi("test" , "shade run %d" , shader.programId);
    // // std::cout << "error 0 " << glGetError() << std::endl;
    // glBindBuffer(GL_ARRAY_BUFFER , vbo);
    // // std::cout << "error 1 " << glGetError() << std::endl;
    // glEnableVertexAttribArray(0);
    // // std::cout << "error 2 " << glGetError() << std::endl;
    // glVertexAttribPointer(0 , 2 , GL_FLOAT , GL_FALSE , 2 * sizeof(float) , (void *)0);
    // std::cout << "error 3 " << glGetError() << std::endl;
    // std::cout << std::endl;
    
    // glDrawArrays(GL_TRIANGLE_FAN , 0 , 3);
    // glBindBuffer(GL_ARRAY_BUFFER , 0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN , 0 , 3);
    // std::cout << "error " << glGetError() << std::endl;
}

void Triangle::free(){
    ShaderManager::getInstance()->clear();
}