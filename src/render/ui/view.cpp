
#include "view.hpp"
#include "../render.hpp"

void View::onRender(std::shared_ptr<RenderEngine> renderEngine){
    Logi("view" , "view on render");
}

void ViewGroup::onRender(std::shared_ptr<RenderEngine> renderEngine){
    //todo render self

    //render child
    for(auto &childView : childViews_){
        childView->onRender(renderEngine);
    }//end for each
}

void ViewGroup::addView(std::shared_ptr<View> view){
    childViews_.push_back(view);
}


