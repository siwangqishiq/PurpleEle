#pragma once

#include "log.hpp"
#include "../common.hpp"
#include <memory>
#include <vector>

class RenderEngine;

//UI base class
class View{
public:
    Rect viewRect;
    virtual void onRender(std::shared_ptr<RenderEngine> renderEngine);
};

//ViewGroup
class ViewGroup : public View{
public:
    virtual void onRender(std::shared_ptr<RenderEngine> renderEngine);

    virtual void addView(std::shared_ptr<View> view);
private:
    std::vector<std::shared_ptr<View>> childViews_;
};


