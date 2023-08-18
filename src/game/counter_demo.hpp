#pragma once

#include "application.hpp"
#include "utils.hpp"
#include "render/ui/view.hpp"
#include <memory>

class CounterDemo : public IScene,EventActionCallback {
public:
    CounterDemo(Application *app){
        appContext = app;
    }

    glm::vec4 COLOR_SKY_BLUE = ConvertColor(33,150,243,255);
    glm::vec4 COLOR_WHITE = ConvertColor(255,255,255,255);
    glm::vec4 COLOR_GRAY = ConvertColor(115 , 115 , 115 , 255);

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
    
    //UI root view
    std::shared_ptr<RootViewGroup> rootView_;

    std::shared_ptr<View> roundButton_;
    std::shared_ptr<TextView> countText_;

    unsigned int count_ = 0;
};