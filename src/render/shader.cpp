//
// Created by panyi on 2022/7/1.
//

#include "shader.hpp"
#include <fstream>
#include "glm/gtc/type_ptr.hpp"
#include "log.hpp"
#include "resource/asset_manager.hpp"

std::string ReadAssetTextFile(std::string filename) {
    return std::string();
}   

GLuint CreateGPUProgram(const char* vsShaderSource, const char* fsShaderSource) {
    GLuint vsShader = CompileShader(GL_VERTEX_SHADER, vsShaderSource);
    GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, fsShaderSource);
    // std::cout << "glCompileProgram e222:" << glGetError() <<std::endl;
    //Attach
    GLuint program = glCreateProgram();
    glAttachShader(program, vsShader);
    glAttachShader(program, fsShader);
    // std::cout << "glCompileProgram e111:" << glGetError() <<std::endl;
    
    // GLint compileResult = GL_TRUE;
    // glGetProgramiv(program, GL_COMPILE_STATUS, &compileResult);
    // std::cout << "glCompileProgram e0000:" << glGetError() <<std::endl;
    // if(compileResult == GL_FALSE){
    //     char szLog[1024] = { 0 };
    //     GLsizei logLen = 0;
    //     glGetProgramInfoLog(program, 1024, &logLen, szLog);
    //     Logi(TAG_SHADER, "Compile program fail error log: %s \nvs shader code:\n%s\nfs shader code:\n%s\n" , szLog , vsShaderSource , fsShaderSource);
    //     glDeleteShader(program);
    //     program = 0;
    // }

    //Link
    glLinkProgram(program);
    GLint linkResult = GL_TRUE;
    glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
    if(linkResult == GL_FALSE){
        char szLog[1024] = { 0 };
        GLsizei logLen = 0;
        glGetProgramInfoLog(program, 1024, &logLen, szLog);
        Logi(TAG_SHADER, "Compile program fail error log: %s \nvs shader code:\n%s\nfs shader code:\n%s\n" , szLog , vsShaderSource , fsShaderSource);
        glDeleteShader(program);
        program = 0;
    }

    // std::cout << "glLinkProgram e000:" << glGetError() <<std::endl;

    //Clear
    glDetachShader(program, vsShader);
    glDetachShader(program, fsShader);
    glDeleteShader(vsShader);
    glDeleteShader(fsShader);

    // std::cout << "glDeleteShader e000:" << glGetError() <<std::endl;
    Logi(TAG_SHADER , "program = %d" , program);
    // std::cout << "glCompileProgram error:" << glGetError() <<std::endl;
    return program;
}

//读取文本文件
std::string ReadFileAsText(std::string path){
    std::string content;
    std::ifstream infile(path);
    if(infile.is_open()){
        std::string line;
        while(std::getline(infile , line)){
            line += "\n";
            content += line;
        }//end while
    }else{
        Logi(TAG_SHADER , "open file : %s error" , path.c_str());
    }
    infile.close();
    return content;
}

//从文件中创建shader
GLuint CreateGPUProgramFromFile(std::string vertexShaderPath , std::string fragShaderPath){
    std::string vertexSrc = ReadFileAsText(vertexShaderPath);
    std::string fragSrc = ReadFileAsText(fragShaderPath);

    return CreateGPUProgram(vertexSrc.c_str() , fragSrc.c_str());
}

Shader Shader::buildGPUProgram(std::string vtxSrc , std::string frgSrc){
    Shader shader;
    auto pid = CreateGPUProgram(vtxSrc.c_str() , frgSrc.c_str());
    shader.programId = pid;
    return shader;
}

Shader Shader::buildGPUProgramAssetFile(std::string vtxPath , std::string frgPath){
    Shader shader;
    auto  pid = CreateGPUProgramByAsset(vtxPath , frgPath);
    shader.programId = pid;
    return shader;
}

GLuint CompileShader(GLenum shaderType, const char* shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0) {
        Logi(TAG_SHADER , "create shader failed : %s" , shaderSource);
        glDeleteShader(shader);
        return 0;
    }
    const char* shaderCode = shaderSource;
    if (shaderCode == nullptr) {
        Logi(TAG_SHADER , "load shader code from %s failed" , shaderSource);
        glDeleteShader(shader);
        return 0;
    }

    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);

    GLint compileResult = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE) {
        char szLog[1024] = { 0 };
        GLsizei logLen = 0;
        glGetShaderInfoLog(shader, 1024, &logLen, szLog);
        Logi(TAG_SHADER , "Compile Shader fail error log: %s \nshader code:\n%s\n", szLog, shaderCode);
        glDeleteShader(shader);
        shader = 0;
    }
    return shader;
}

void Shader::useShader(){
    glUseProgram(this->programId);
}

void Shader::setUniformInt(std::string key , int value){
    GLint loc = findUniformLocation(key);
    glUniform1i(loc , value);
}

void Shader::setUniformFloat(std::string key , float value){
    GLint loc = findUniformLocation(key);
    glUniform1f(loc , value);
}

void Shader::setUniformMat3(std::string key, glm::mat3 mat) {
    GLint loc = findUniformLocation(key);
    glUniformMatrix3fv(loc , 1 , GL_FALSE , glm::value_ptr(mat));
}

void Shader::setUniformMat4(std::string key , glm::mat4 mat){
    GLint loc = findUniformLocation(key);
    glUniformMatrix4fv(loc , 1 , GL_FALSE , glm::value_ptr(mat));
}

void Shader::setUniformVec4(std::string key , glm::vec4 vec){
    GLint loc = findUniformLocation(key);
    glUniform4fv(loc , 1 ,  (float *)(&vec[0]));
}

void Shader::setUniformVec3(std::string key , float x , float y , float z){
    GLint loc = findUniformLocation(key);
    glUniform3f(loc , x , y , z);
}

void Shader::setUniformVec3(std::string key ,glm::vec3 value){
    GLint loc = findUniformLocation(key);
    glUniform3fv(loc , 1 , (float *)(&value[0]));
}

void Shader::setUniformVec2(std::string key , float x , float y){
    GLint loc = findUniformLocation(key);
    glUniform2f(loc , x , y);
}

void Shader::setUniformBool(std::string key , bool value){
    GLint loc = findUniformLocation(key);
    glUniform1i(loc , value);
}

int Shader::findUniformLocation(std::string key){
    if(unifromLocs.find(key) != unifromLocs.end()){
        return unifromLocs[key];
    }

    const int loc = glGetUniformLocation(this->programId , key.c_str());
    if(loc >= 0){
        unifromLocs[key] = loc;
    }
    return loc;
}

//获取 或 创建出一个shader
Shader ShaderManager::loadShader(std::string shaderName , 
        std::string vtxSrc , std::string frgSrc) {
    if(shaderMap.find(shaderName) == shaderMap.end()){//not found shader create a new shader
        Logi("Shader" , "create a new shader %s " , shaderName.c_str());
        Shader shader = Shader::buildGPUProgram(vtxSrc , frgSrc);
        shaderMap[shaderName] = shader;
    }

    return shaderMap[shaderName];
}

//获取 或 创建出一个shader
Shader ShaderManager::loadShaderByPath(std::string shaderName , std::string vertexPath , std::string fragPath) {
    if(shaderMap.find(shaderName) == shaderMap.end()){//not found shader create a new shader
        Logi("no found %s , create a new shader" , shaderName.c_str());
        Shader shader = Shader::buildGPUProgramAssetFile(vertexPath , fragPath);
        shaderMap[shaderName] = shader;
    }else{
        Logi("has found %s , reuse shader" , shaderName.c_str());
    }
    return shaderMap[shaderName];
}

void ShaderManager::clear() {
    Logi(TAG_SHADER , "shader manager clear");

    for(auto pair : shaderMap){
        Shader shader = pair.second;
        glDeleteShader(shader.programId);
    }

    Logi(TAG_SHADER ,"shader map size %d" , shaderMap.size());
    shaderMap.clear();
    Logi(TAG_SHADER ,"shader map size after clear %d" , shaderMap.size());
}

std::shared_ptr<ShaderManager> ShaderManager::getInstance() {
    static std::shared_ptr<ShaderManager> instance_;
    if(instance_ == nullptr){
        instance_ = std::make_shared<ShaderManager>();
    }
    return instance_;
}

//从Asset中读取shader
GLuint CreateGPUProgramByAsset(std::string vsFilePath , std::string fsFilePath){
    return CreateGPUProgram(ReadAssetTextFile(vsFilePath).c_str() ,
                            ReadAssetTextFile(fsFilePath).c_str());
}

std::string ShaderManager::readShaderSrc(std::string shaderPath){
    std::string shadrHeadSrc =   
    #ifdef __ANDROID__
    "#version 300 es\n";
    #else
    "#version 330 core\n";
    #endif
    
    auto shaderBodySrc = AssetManager::getInstance()->readTextFileAsString(shaderPath);
    return shadrHeadSrc + shaderBodySrc;
}

Shader ShaderManager::loadAssetShader(std::string shaderName , 
            std::string vertexPath , std::string frgPath){
    return loadShader(shaderName , readShaderSrc(vertexPath) , readShaderSrc(frgPath));
}