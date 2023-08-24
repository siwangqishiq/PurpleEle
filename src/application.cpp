#include "application.hpp"
#include "log.hpp"

//window surface header
#include "glheader.hpp"
#include "render/render.hpp"
#include "resource/asset_manager.hpp"
#include "render/texture.hpp"
#include "render/common.hpp"
#include "widget/timer.hpp"
#include "render/render_batch.hpp"
#include "utils.hpp"
#include "game/test_demo.hpp"
#include "game/shader_demo.hpp"
#include "audio/audio.hpp"
#include "game/ui_demo.hpp"
#include "game/life_game.hpp"
#include <string>
#include "render/ui/view.hpp"
#include "game/counter_demo.hpp"
#include "game/custom_shader_demo.hpp"

void Application::onFree(){
    Logi(TAG , "app onFree");
    eventCallbackList_.clear();

    // if(testDemo_ != nullptr){
    //     testDemo_->dispose();
    // }

    // if(shaderDemo_ != nullptr){
    //     shaderDemo_->dispose();
    // }

    if(currentScene_ != nullptr){
        currentScene_->dispose();
    }

    if(timer_ != nullptr){
        timer_->clear();
    }

    AudioManager::getInstance()->dispose();
    
    // triangleDemo_->free();
    if(renderEngine_ != nullptr){
        renderEngine_->free();
    }
}   

void Application::init(){
    onInit();
}


void Application::update(){
    onTick();
}

void Application::free(){
    onFree();
}

Application::~Application(){
    Logi(TAG , "~app Application");
}

void Application::onResize(int w , int h){
    Logi(TAG , "app onresize %d , %d" , w , h);
    screenWidth_ = w;
    screenHeight_ = h;

    viewWidth_ = w;
    viewHeight_ = h;
    
    if(renderEngine_ != nullptr){
        renderEngine_->onScreenResize();
    }

    createRootView();

    if(currentScene_ != nullptr){
        currentScene_->init();
    }
}

std::shared_ptr<Timer> Application::getTimer(){
    if(timer_ == nullptr){
        timer_ = std::make_shared<Timer>();
    }

    return timer_;
}

void Application::onInit(){
    Logi(TAG , "app onInit");

    // renderEngine_ = std::shared_ptr<RenderEngine>(new RenderEngine(std::shared_ptr<Application>(this)));

    renderEngine_ = std::make_shared<RenderEngine>(this);
    renderEngine_->init();
    
    renderEngine_->onScreenResize();

    AudioManager::getInstance()->init();

    isShowFps = true;
    timeStamp_ = currentTimeMicro();

    createRootView();

    onCreate();
}

 void Application::onEventAction(int event , float x , float y){
    // Logi("action" , "tigger action: %d (%f, %f)" , event , x , y);
    y = viewHeight_ - y;
    
    for(EventActionCallback *pCallback : eventCallbackList_){
        if(pCallback != nullptr){
            bool isCost = pCallback->onEventAction(event , x , y);
            if(isCost){
                break;
            }
        }
    }//end for each
 }

void Application::onCreate(){
    getTimer()->scheduleAtFixedRate([this](Application *app){
        Logi("application" , "timer fps : %d" , frameCount_);
        showFps = frameCount_;
        frameCount_ = 0;
    } , 1000L);

  
    startTime_ = static_cast<long>(currentTimeMillis());

    // currentScene_ = std::make_shared<TestDemo>(this);
    // currentScene_ = std::make_shared<UiDemo>(this);
    // currentScene_ = std::make_shared<CounterDemo>(this);
    currentScene_ = std::make_shared<LifeGame>(this);
    // currentScene_ = std::make_shared<CustomShaderDemo>(this);
    currentScene_->init();
}

void Application::createRootView(){
    rootViewGroup_ = std::make_shared<RootViewGroup>(viewWidth_ , viewHeight_);
    rootViewGroup_->setTag("rootView");
}

void Application::onTick(){
    // Log(TAG , "app trick");
    // Logi(TAG , "getLastFrameDeltaTimeMirco = %lld" , getLastFrameDeltaTime());
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    long long timeStart = currentTimeMillis();
    if(renderEngine_ == nullptr){
        return;
    }

    //user logic and draw
    updateSence();
    
    //定时器triker
    if(timer_ != nullptr){
        timer_->trick(this);
    }

    //gl commands run
    renderEngine_->render();

    // for test
    // triangleDemo_->trick(renderEngine_->normalMatrix_);

    timeStamp_ = currentTimeMicro();
    long timeEnd = currentTimeMillis();
    auto deltaTime = timeEnd - timeStart;
    frameCount_++;

    // Logi(TAG , "frame cost time : %ld" , deltaTime);
}

void Application::updateSence(){
    currentScene_->tick();

    if(isShowFps){
        TextPaint p4;
        p4.setTextSize(viewHeight_ / 20.0f);
        p4.textColor = showFps >=50
            ? glm::vec4(0.0f ,1.0f , 0.0f , 1.0f)
            : glm::vec4(1.0f , 0.0f , 0.0f , 1.0f);
        std::wstring name = L"fps:";
        std::wstring fpsStr = name + std::to_wstring(showFps);

        Rect outputRect;
        outputRect.left = 0.0f;
        outputRect.top = viewHeight_ - 16.0f;
        outputRect.width = viewWidth_ - 16.0f;
        outputRect.height = viewHeight_;
        p4.textGravity = TopRight; 
        
        renderEngine_->renderTextWithRect(fpsStr , outputRect , p4 , nullptr);
        // renderEngine_->renderText(fpsStr, screenWidth_ - 220.0f, 
        //     screenHeight_ - 80.0f, p4);
    }
}


long long Application::getLastFrameDeltaTime(){
    // if(timeStamp_ <= 0 ){
    //     return 0L;
    // }
    return currentTimeMicro() - timeStamp_;
}

bool Application::checkInCallbackList(EventActionCallback *callback){
    for(auto p : eventCallbackList_){
        if(p == callback){
            return true;
        }
    }
    return false;
}

bool Application::addEventActionCallback(EventActionCallback *callback){
    if(checkInCallbackList(callback)){
        return false;
    }

    eventCallbackList_.push_back(callback);
    Logi("application" , "callback list size %d" , eventCallbackList_.size());
    return true;
}

EventActionCallback* Application::removeEventActionCallback(EventActionCallback *callback){
    if(!checkInCallbackList(callback)){
        return nullptr;
    }

    for(int i = 0 ; i < eventCallbackList_.size();i++){
        if(callback == eventCallbackList_[i]){
            eventCallbackList_.erase(eventCallbackList_.begin() + i);
            break;
        }
    }

    Logi("application" , "callback list size %d" , eventCallbackList_.size());
    return callback;
}



