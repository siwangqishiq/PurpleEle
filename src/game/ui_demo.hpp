#pragma once

#include "application.hpp"
#include "utils.hpp"

class RenderEngine;
class TextureImage;
struct AudioEntity;

class UiDemo :public EventActionCallback{
public:
    UiDemo(Application *app){
        appContext = app;
    }

    void init();

    void tick();

    void dispose();

    virtual bool onEventAction(int action , float x , float y);

    bool isInited = false;

    glm::vec4 COLOR_SKY_BLUE = ConvertColor(100,181,246,255);

    virtual void buildViews();
private:
    Application *appContext;

    int viewWidth_;
    int viewHeight_;

    std::shared_ptr<RenderEngine> renderEngine_;
};