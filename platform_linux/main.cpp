#include <iostream>
#include "libjson/json.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "application.hpp"

class LinuxApplication : public Application{
};


int main(int argc , char *argv[]){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //todo create instance
    std::unique_ptr<LinuxApplication> app = std::make_unique<LinuxApplication>();


    GLFWwindow* window = glfwCreateWindow(
                            app->viewWidth_, 
                            app->viewHeight_, 
                            "run", nullptr, nullptr);
    char *buf;
    glfwGetError((const char **)&buf);
    std::cout << buf << std::endl;
    
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window , &app);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* windows_,int w,int h){
        void* app_ = glfwGetWindowUserPointer(windows_);
        std::shared_ptr<LinuxApplication> app= 
            *(static_cast<std::shared_ptr<LinuxApplication> *>(app_));
        app->onResize(w , h);
    });

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    app->onResize(app->viewWidth_ , app->viewHeight_);
    app->init();
    while (!glfwWindowShouldClose(window)) {
        // std::cout << "tick" << std::endl;
        app->update();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }//end while
    
    app->onFree();
	glfwTerminate();
    return 0;
}
