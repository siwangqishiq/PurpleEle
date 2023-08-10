#include "ui_demo.hpp"
#include "utils.hpp"
#include "log.hpp"

void UiDemo::init(){
    viewWidth_ = appContext->viewWidth_;
    viewHeight_ = appContext->viewHeight_;

    renderEngine_ = appContext->getRender();
    appContext->addEventActionCallback(this);

    if(isInited){
        return;
    }
    isInited = true;

    buildViews();
}

void UiDemo::buildViews(){
    Logi("UiDemo" , "buildViews");
}

void UiDemo::tick(){

}

void UiDemo::dispose(){
    Logi("UiDemo" , "dispose");
}



bool UiDemo::onEventAction(int action , float x , float y){
    Logi("UiDemo" , "onEventAction event: %d ( %f , %f)" , action , x , y);
    return false;
}


