#pragma once

#include "application.hpp"
#include "utils.hpp"
#include "render/ui/view.hpp"

class RenderEngine;
// class View;
// class ViewGroup;
// class IViewListener;

class UiDemo :public IScene,EventActionCallback , IViewListener{
public:
    UiDemo(Application *app){
        appContext = app;
    }

    virtual void init();

    virtual void tick();

    virtual void dispose();

    virtual bool onEventAction(int action , float x , float y);

    bool isInited = false;

    glm::vec4 COLOR_SKY_BLUE = ConvertColor(100,181,246,255);

    virtual void buildViews();

    virtual void onClick(View *view) override;
private:
    Application *appContext;

    int viewWidth_;
    int viewHeight_;

    std::shared_ptr<RenderEngine> renderEngine_;

    //UI root view
    std::shared_ptr<RootViewGroup> rootView_;

    std::shared_ptr<View> childView1_;
    std::shared_ptr<View> childView2_;
    std::shared_ptr<View> childView3_;

    std::shared_ptr<ViewGroup> viewContainer_;
    std::shared_ptr<View> childView4_;
};