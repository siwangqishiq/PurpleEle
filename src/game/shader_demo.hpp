#pragma once

#include "application.hpp"

class RenderEngine;

class ShaderDemo : public IScene{
public:
    ShaderDemo(Application *app){
        appContext = app;
    }

    virtual void init();
    
    virtual void tick();

    virtual void dispose();

    bool isInited = false;
private:
    Application *appContext;

    int viewWidth_;
    int viewHeight_;

    std::shared_ptr<RenderEngine> renderEngine_;
    
    Shader customRenderShader;
    void testRenderShader();
};