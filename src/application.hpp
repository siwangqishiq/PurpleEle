#pragma once

#include <string>
#include <memory>
#include "render/triangle.hpp"
#include <ctime>
#include <chrono>
#include <sstream>


class RenderEngine;
class Timer;

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

    int fixedRateTaskId = -1;

    int frameCount_ = 0;

    //获取上一帧经过的时间
    long long getLastFrameDeltaTime();
    long long timeStamp_ = -1L;
private:
    std::shared_ptr<RenderEngine> renderEngine_ = nullptr;

    //for -----------test---------------------
    std::shared_ptr<Triangle> triangleDemo_ = nullptr;

    //定时器  调度定时任务 或 延时任务
    std::shared_ptr<Timer> timer_ = nullptr;
    
    float x_ = 0.0f;
    float y_ = 0.0f;

    int textIndex = 0;

    float mScale = 1.0f;
    int mIndex = 1;
    bool showNumber = false; 
    int showFps = 0;

    std::wstring showTextContent;
    long startTime = 0;

    Shader testShader;
    Shader testShader2;
    float mRadius = 1.0f;

    void testRender1();
    void testRender2();
    void testRender3();
    void testRender4();
    void testRenderRoundRect();
    void testRenderTableTennis();
};