#include <iostream>
#include "libjson/json.hpp"

//window surface header
#ifdef _WIN32
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#elif _WIN64
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#elif __CYGWIN__
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

//for windows application
class WinApplication : public Application{
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
    std::shared_ptr<WinApplication> app = std::make_shared<WinApplication>();

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
        std::shared_ptr<WinApplication> app= 
            *(static_cast<std::shared_ptr<WinApplication> *>(app_));
        app->onResize(w , h);
    });

    glfwSetMouseButtonCallback(window , [](GLFWwindow* windows_,int button,int event,int mods){
        void* app_ = glfwGetWindowUserPointer(windows_);
        std::shared_ptr<WinApplication> app= 
            *(static_cast<std::shared_ptr<WinApplication> *>(app_));
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
            std::shared_ptr<WinApplication> app= 
                *(static_cast<std::shared_ptr<WinApplication> *>(app_));
            app->onEventAction(ACTION_MOVE , mouseX , mouseY);
        }
        // std::cout << "pos: " << mouseX << "  " << mouseY << std::endl;
    });

    glfwSetKeyCallback(window , [](GLFWwindow* windows_,int key,int scancode,int action,int mods){
        // std::cout << "key " << key << "  scancode " << scancode << " action " << action << std::endl;
    });

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //force call resize
    app->onResize(app->viewWidth_ , app->viewHeight_);
    app->init();
    while (!glfwWindowShouldClose(window)) {
        // std::cout << "tick" << std::endl;
        app->update();
            
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        glfwSwapInterval(1);//锁定固定帧率
    }//end while
    
    app->onFree();
	glfwTerminate();
    return 0;
}
