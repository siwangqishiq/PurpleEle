#pragma once

#include "application.hpp"
#include "utils.hpp"
#include <memory>

class CustomShaderDemo : public IScene,EventActionCallback {
public:
    CustomShaderDemo(Application *app){
        appContext = app;
    }

    bool isInited = false;
    
    virtual void init() override;

    virtual void tick() override;

    virtual void dispose() override;

    virtual bool onEventAction(int action , float x , float y) override;

    void buildViews();
private:
    Application *appContext;

    int viewWidth_;
    int viewHeight_;

    std::shared_ptr<RenderEngine> renderEngine_;

    Shader customRenderShader_;
};