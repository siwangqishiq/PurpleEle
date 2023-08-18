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

    virtual void init() override;

    virtual void tick() override;

    virtual void dispose() override;

    virtual bool onEventAction(int action , float x , float y) override;

    bool isInited = false;

    glm::vec4 COLOR_SKY_BLUE = ConvertColor(100,181,246,255);
    glm::vec4 COLOR_WHITE = ConvertColor(255,255,255,255);

    void test1();

    void test2();

    void test3();

    void test4();

    void test5();

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

    std::shared_ptr<TextView> textView1_;

    std::shared_ptr<View> bottomView_;

    std::shared_ptr<View> rectView_;

    std::shared_ptr<TextView> testTextView_;

    std::shared_ptr<ButtonView> testButtonView_;
};