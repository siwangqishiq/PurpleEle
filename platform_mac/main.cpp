#include <iostream>
#include "libjson/json.hpp"

//window surface header
#ifdef _WIN32
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#elif _WIN64
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#elif __ANDROID__
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#elif __APPLE__
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

#include "application.hpp"
#include "input/input_manager.hpp"

class MacApplication : public Application{
};

static bool MouseActionDown = false;
static int mouseX = 0;
static int mouseY = 0;

int main(int argc , char *argv[]){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //todo create instance
    std::shared_ptr<MacApplication> app = std::make_shared<MacApplication>();

    GLFWwindow* window = glfwCreateWindow(
                            app->viewWidth_, 
                            app->viewHeight_, 
                            "run", nullptr, nullptr);
    
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window , &app);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* windows_,int w,int h){
        void* app_ = glfwGetWindowUserPointer(windows_);
        std::shared_ptr<MacApplication> app= 
            *(static_cast<std::shared_ptr<MacApplication> *>(app_));
        app->onResize(w , h);
    });

    glfwSetMouseButtonCallback(window , [](GLFWwindow* windows_,int button,int event,int mods){
        void* app_ = glfwGetWindowUserPointer(windows_);
        std::shared_ptr<MacApplication> app= 
            *(static_cast<std::shared_ptr<MacApplication> *>(app_));
        if(event == GLFW_PRESS){
            MouseActionDown = true;
            app->onEventAction(ACTION_DOWN , mouseX , mouseY);
        }else if(event == GLFW_RELEASE){
            MouseActionDown = false;
            app->onEventAction(ACTION_UP , mouseX , mouseY);
        }
        // std::cout << "event " << button << "  " << event << std::endl;
    });

    glfwSetCursorPosCallback(window , [](GLFWwindow* windows_,double x,double y){
        mouseX = static_cast<int>(x);
        mouseY = static_cast<int>(y);

        if(MouseActionDown){
            void* app_ = glfwGetWindowUserPointer(windows_);
            std::shared_ptr<MacApplication> app= 
                *(static_cast<std::shared_ptr<MacApplication> *>(app_));
            app->onEventAction(ACTION_MOVE , mouseX , mouseY);
        }
        // std::cout << "pos: " << mouseX << "  " << mouseY << std::endl;
    });

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //force call resize
    app->onResize(app->screenWidth_ , app->screenHeight_);
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
