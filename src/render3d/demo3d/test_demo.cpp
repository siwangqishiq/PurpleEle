#include "test_demo.hpp"
#include <memory>

void Test3dDemo::init(){
    viewWidth_ = appContext->viewWidth_;
    viewHeight_ = appContext->viewHeight_;

    renderEngine_ = appContext->getRender();
    appContext->isShowFps = true;
    
    //

    if(isInited){
        return;
    }
    isInited = true;
}

void Test3dDemo::tick(){
    
}

void Test3dDemo::dispose(){

}



