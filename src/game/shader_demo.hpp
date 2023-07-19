#pragma once

#include "application.hpp"

class RenderEngine;

class ShaderDemo{
public:
    ShaderDemo(Application *app){
        appContext = app;
    }

    void init();
    
    void tick();

    void dispose();

    bool isInited = false;
private:
    Application *appContext;

    int viewWidth_;
    int viewHeight_;

    std::shared_ptr<RenderEngine> renderEngine_;
    
    Shader customRenderShader;
    void testRenderShader();
};