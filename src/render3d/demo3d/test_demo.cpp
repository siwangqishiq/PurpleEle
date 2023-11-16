#include "test_demo.hpp"
#include <memory>
#include "../../render/render.hpp"

void Test3dDemo::init(){
    viewWidth_ = appContext->viewWidth_;
    viewHeight_ = appContext->viewHeight_;

    renderEngine_ = appContext->getRender();
    appContext->isShowFps = true;
    
    if(isInited){
        return;
    }
    isInited = true;
}

void Test3dDemo::tick(){
    Rect rect(0 , appContext->viewHeight_, 
        appContext->viewWidth_ , 
        appContext->viewHeight_);
    TextPaint textPaint;
    textPaint.textGravity = TextGravity::Center; 
    renderEngine_->renderTextWithRect(L"Hello 3D World!" , rect ,textPaint , nullptr);
}

void Test3dDemo::dispose(){

}



