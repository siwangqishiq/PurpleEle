#include "ui_demo.hpp"
#include "utils.hpp"
#include "log.hpp"
#include "audio/audio.hpp"
#include "render/ui/view.hpp"

void UiDemo::init(){
    viewWidth_ = appContext->viewWidth_;
    viewHeight_ = appContext->viewHeight_;

    renderEngine_ = appContext->getRender();
    appContext->addEventActionCallback(this);
    
    //
    rootView_ = appContext->getRootView();

    if(isInited){
        return;
    }
    isInited = true;

    // AudioManager::getInstance()->loadAudio("audio/butterfly.mp3","bgm",true);

    buildViews();
    // AudioManager::getInstance()->playAudio("bgm");
}

void UiDemo::buildViews(){
    Logi("UiDemo" , "buildViews");
    rootView_->setBackgroundColor(glm::vec4(0.0f , 0.0f , 0.0f , 0.0f));

    int posX = 100;
    int posY = -100;
    childView1_ = std::make_shared<View>(500 , 600);
    childView1_->setBackgroundColor(glm::vec4(1.0f , 0.0f , 0.0f , 1.0f));
    childView1_->setTag("red view");
    rootView_->addView(childView1_ , posX , posY);

    childView2_ = std::make_shared<View>(500 , 600);
    childView2_->setBackgroundColor(glm::vec4(0.0f , 1.0f , 0.0f , 1.0f));
    childView2_->setTag("green view");
    posX += 20;
    posY -= 20;
    rootView_->addView(childView2_ , posX , posY);

    childView3_ = std::make_shared<View>(500 , 600);
    childView3_->setBackgroundColor(glm::vec4(0.0f , 0.0f , 1.0f , 1.0f));
    childView3_->setTag("blue view");
    posX += 20;
    posY -= 20;
    rootView_->addView(childView3_ , posX , posY);

    viewContainer_ = std::make_shared<ViewGroup>(500 , 500);
    viewContainer_->setBackgroundColor(glm::vec4(1.0f , 0.0f , 1.0f , 0.5f));
    viewContainer_->setTag("view container");
    rootView_->addView(viewContainer_ , 600 , -100);

    childView4_ = std::make_shared<View>(300 , 300);
    childView4_->setTag("view4");
    childView4_->setBackgroundColor(glm::vec4(0.0f , 1.0f , 1.0f , 1.0f));
    auto containerSize = viewContainer_->getViewRect();
    auto childView4Size = childView4_->getViewRect();
    viewContainer_->addView(childView4_ , 
        containerSize.width / 2  - childView4Size.width / 2  , 
        -containerSize.width / 2 + childView4Size.height / 2);
}

void UiDemo::tick(){
    rootView_->onRender(renderEngine_); //UI render
}

void UiDemo::dispose(){
    Logi("UiDemo" , "dispose");
}

bool UiDemo::onEventAction(int action , float x , float y){
    // Logi("UiDemo" , "onEventAction event: %d ( %f , %f)" , action , x , y);
    bool cost = rootView_->dispathTouchEvent(action , x , y);

    if(!cost){
        //todo your owen logic
    }
    return cost;
}


