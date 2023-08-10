#pragma once

#include <string>
#include <memory>
#include "render/triangle.hpp"
#include <ctime>
#include <chrono>
#include <sstream>
#include <vector>


class RenderEngine;
class Timer;
class TestDemo;
class ShaderDemo;
class UiDemo;

class EventActionCallback{
public:
    virtual bool onEventAction(int action , float x , float y) = 0;
};

class IScene{
public:
    virtual void init() = 0;

    virtual void tick() = 0;

    virtual void dispose() = 0;
};

/**
 * @brief  应用入口
 * 
 */
class Application {
public:
    const std::string TAG = "Application"; 

    int viewWidth_ = 1280;
    int viewHeight_ = 800;

    int screenWidth_ = viewWidth_;
    int screenHeight_ = viewHeight_;
    
    void onInit();

    void onTick();

    void onFree();

    void onCreate();

    void onEventAction(int event , float x , float y);

    bool addEventActionCallback(EventActionCallback *callback);

    EventActionCallback* removeEventActionCallback(EventActionCallback *callback);
    
    virtual void updateSence();
    
    virtual void onResize(int width , int height);
    
    //平台相关
    virtual void init();

    virtual void update();

    virtual void free();

    ~Application();

    std::shared_ptr<RenderEngine> getRender() {
        return renderEngine_;
    }
    
    std::shared_ptr<Timer> getTimer();

    int frameCount_ = 0;

    //获取上一帧经过的时间
    long long getLastFrameDeltaTime();
    long long timeStamp_ = -1L;
    
protected:
    long startTime_;

    //渲染器
    std::shared_ptr<RenderEngine> renderEngine_ = nullptr;
    //定时器  调度定时任务 或 延时任务
    std::shared_ptr<Timer> timer_ = nullptr;
private:
    std::vector<EventActionCallback *> eventCallbackList_;

    bool checkInCallbackList(EventActionCallback *callback);
    //for -----------test---------------------
    
    std::shared_ptr<IScene> currentScene_ = nullptr;

    bool showNumber = false;
    int showFps = 0;

};