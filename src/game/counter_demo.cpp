#include "counter_demo.hpp"
#include "../render/render_batch.hpp"
#include "../render/ui/view.hpp"
#include <memory>

void CounterDemo::init(){
    viewWidth_ = appContext->viewWidth_;
    viewHeight_ = appContext->viewHeight_;

    renderEngine_ = appContext->getRender();
    appContext->addEventActionCallback(this);
    appContext->isShowFps = true;
    
    //
    rootView_ = appContext->getRootView();
    buildViews();

    if(isInited){
        return;
    }
    isInited = true;
}

void CounterDemo::buildViews(){
    rootView_->setBackgroundColor(glm::vec4(1.0f ,1.0f ,1.0f ,1.0f));

    int bannerHeight = viewHeight_ / 9.0f;
    std::shared_ptr<View> topBanner = std::make_shared<View>(viewWidth_, bannerHeight);
    topBanner->setBackgroundShadowRoundRect(COLOR_SKY_BLUE , 1.0f , 8.0f);
    rootView_->addView(topBanner , 0 , 0);

    std::shared_ptr<TextView> topTextView = std::make_shared<TextView>(viewWidth_ , bannerHeight);
    topTextView->setText(L"PurpleEle Demo Home Page");
    topTextView->setTextSize(bannerHeight / 3.5f);
    topTextView->setTextColor(COLOR_WHITE);
    topTextView->setTextGravity(CenterLeft);
    rootView_->addView(topTextView , 30.0f , 0.0f);
    
    int btnSize = bannerHeight;
    int padding = 20;
    roundButton_ = std::make_shared<View>(btnSize , btnSize);
    // roundButton_->setBackgroundShadowRoundRect(COLOR_SKY_BLUE , btnSize / 2.0f , 8.0f);
    rootView_->addView(roundButton_ , viewWidth_ - btnSize - padding
                         , -viewHeight_ + btnSize + padding);
    
    roundButton_->setCustomRenderPass([this](std::shared_ptr<RenderEngine> renderEngine){
        auto viewRect = roundButton_->getViewRect().toRectF();
        auto batch = renderEngine->getShapeBatch();
        batch->begin();

        Paint paint;
        
        float radius = viewRect.width / 2.0f;
        auto p = viewRect.center();

        if(roundButton_->getViewState() == ViewState::IDLE){
            paint.color = COLOR_BLACK;
            batch->renderBlurCircle(p.x , p.y , radius - 8.0f , 12.0f , paint);
        }

        paint.color = COLOR_SKY_BLUE;
        batch->renderCircle(p.x ,p.y , radius, paint);

        paint.color = COLOR_WHITE;
        float lineWidth = radius / 2.0f;
        float lineHeight = radius / 10.0f;

        Rect vertRect;
        vertRect.left = p.x - lineHeight / 2.0f;
        vertRect.top = p.y + lineWidth / 2.0f;
        vertRect.width = lineHeight;
        vertRect.height = lineWidth;
        batch->renderRect(vertRect, paint);

        Rect horRect;
        horRect.left = p.x - lineWidth / 2.0f;
        horRect.top = p.y + lineHeight / 2.0f;
        horRect.width = lineWidth;
        horRect.height = lineHeight;
        batch->renderRect(horRect, paint);

        batch->end();
    });

    roundButton_->setLambdaOnClickListener([this](View *view){
        Logi("couner_demo" , "button clicked!");
        
        count_++;
        std::wstring str = std::to_wstring(count_);
        countText_->setText(str);
    });

    int tipTextHeight = bannerHeight / 2.0f;
    auto tipsText = std::make_shared<TextView>(viewWidth_ , tipTextHeight);
    tipsText->setTextColor(COLOR_GRAY);
    tipsText->setText(L"You have pushed the button this many times:");
    tipsText->setTextGravity(Center);
    tipsText->setTextSize(tipTextHeight / 2.1f);
    rootView_->addView(tipsText , 0 , -viewHeight_ / 2.0f + tipTextHeight);
    
    int countTextHeight = tipTextHeight * 2.0f;
    countText_ = std::make_shared<TextView>(viewWidth_ , countTextHeight);
    countText_->setTextColor(COLOR_GRAY);
    std::wstring str = std::to_wstring(count_);
    countText_->setText(str);
    countText_->setTextGravity(TopCenter);
    countText_->setTextSize(countTextHeight / 2.0f);
    rootView_->addView(countText_ , 0 , -viewHeight_ / 2.0f);
}

void CounterDemo::tick(){
    rootView_->onRender(renderEngine_); //UI render
}

void CounterDemo::dispose(){

}

bool CounterDemo::onEventAction(int action , float x , float y){
    bool cost = rootView_->dispatchTouchEvent(action , x , y);
    if(!cost){
    }
    return cost;
}



