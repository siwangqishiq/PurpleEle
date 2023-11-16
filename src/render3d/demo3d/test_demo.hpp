#pragma once

#include "application.hpp"
#include "utils.hpp"
#include <memory>

class Test3dDemo : public IScene {
public:
    Test3dDemo(Application *app){
        appContext = app;
    }

    bool isInited = false;
    
    virtual void init() override;

    virtual void tick() override;

    virtual void dispose() override;
private:
    Application *appContext;

    int viewWidth_;
    int viewHeight_;

    std::shared_ptr<RenderEngine> renderEngine_;
};