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
    buildViews();

    if(isInited){
        return;
    }
    isInited = true;



    // AudioManager::getInstance()->loadAudio("audio/butterfly.mp3","bgm",true);
    // AudioManager::getInstance()->playAudio("bgm");
}

void UiDemo::test1(){
    Logi("UiDemo" , "buildViews test1");
    rootView_->clearAllChildViews();

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

    childView1_->setOnClickListener(this);
    childView4_->setOnClickListener(this);
    viewContainer_->setOnClickListener(this);

    childView3_->setLambdaOnClickListener([this](View *view){
        onClick(view);
    });
}

void UiDemo::test2(){
    Logi("UiDemo" , "buildViews test2");
    // rootView_->setBackgroundColor(glm::vec4(0.0f , 0.0f , 0.0f , 0.0f));
    rootView_->clearAllChildViews();

    textView1_ = std::make_shared<TextView>(400 , 300);
    // textView1_->setBackgroundColor(glm::vec4(0.0f , 0.0f , 0.0f , 0.0f));
    textView1_->setText(L"你好世界\nHello World");
    textView1_->setTextColor(glm::vec4(0.0f , 1.0f , 0.0f , 1.0f));
    textView1_->setTextGravity(Center);
    rootView_->addView(textView1_ , viewWidth_ / 2 - textView1_->getViewRect().width / 2 
        , -viewHeight_ / 2 + textView1_->getViewRect().height / 2);

    textView1_->setLambdaOnClickListener([this](View *view){
        textView1_->setTextColor(glm::vec4(1.0f , 0.0f , 0.0f ,1.0f));
        textView1_->setText(L"Hello\nWorld");

        textView1_->setBackgroundColor(glm::vec4(0.0f , 0.0f , 0.0f , 0.5f));
    });
    
    textView1_->setBackgroundColor(glm::vec4(0.0f, 0.0f,1.0f, 0.4f));
}

void UiDemo::test3(){
    rootView_->clearAllChildViews();

    bottomView_ = std::make_shared<View>(viewHeight_ , viewHeight_);
    bottomView_->setBackgroundColor(glm::vec4(1.0f , 1.0f , 1.0f ,1.0f));
    rootView_->addView(bottomView_ 
        , viewWidth_ / 2 - bottomView_->getViewRect().width / 2 
        , -viewHeight_ / 2 + bottomView_->getViewRect().height / 2);

    rectView_ = std::make_shared<View>(300 , 200);
    rectView_->setBackgroundRoundRect(glm::vec4(0.0f , 1.0f , 0.0f , 1.0f) , 40.0f);
    rootView_->addView(rectView_ 
        , viewWidth_ / 2 - rectView_->getViewRect().width / 2 
        , -viewHeight_ / 2 + rectView_->getViewRect().height / 2);
}

void UiDemo::test4(){
    rootView_->clearAllChildViews();

    bottomView_ = std::make_shared<View>(viewHeight_ , viewHeight_);
    bottomView_->setBackgroundColor(glm::vec4(1.0f , 1.0f , 1.0f ,1.0f));
    rootView_->addView(bottomView_ 
        , viewWidth_ / 2 - bottomView_->getViewRect().width / 2 
        , -viewHeight_ / 2 + bottomView_->getViewRect().height / 2);

    testTextView_ = std::make_shared<TextView>(300 , 150);
    testTextView_->setBackgroundShadowRoundRect(COLOR_SKY_BLUE , 0.0f , 8.0f);
    testTextView_->setText(L"Hello你好");
    testTextView_->setTextColor(COLOR_WHITE);
    testTextView_->setTextGravity(Center);

    testTextView_->setViewStateChangeListener([this](View *view ,ViewState state){
        Logi("view" , "view state change %d" , state);
    });

    rootView_->addView(testTextView_ 
        , viewWidth_ / 2 - testTextView_->getViewRect().width / 2 
        , -viewHeight_ / 2 + testTextView_->getViewRect().height / 2);
}

void UiDemo::test5(){
    rootView_->clearAllChildViews();

    bottomView_ = std::make_shared<View>(viewHeight_ , viewHeight_);
    bottomView_->setBackgroundColor(glm::vec4(1.0f , 1.0f , 1.0f ,1.0f));
    rootView_->addView(bottomView_ 
        , viewWidth_ / 2 - bottomView_->getViewRect().width / 2 
        , 0);

    testButtonView_ = std::make_shared<ButtonView>(300 , 100 , L"点我");
    
    testButtonView_->setButtonUI(COLOR_SKY_BLUE , 8.0f , 4.0f);
    testButtonView_->setTextSize(40.0f);
    testButtonView_->setLambdaOnClickListener([this](View *view){
        Logi("view" , "Button clicked!");
    });

    rootView_->addView(testButtonView_ 
        , viewWidth_ / 2.0f - testButtonView_->getViewRect().width / 2
        , -viewHeight_ / 2.0f + testButtonView_->getViewRect().height / 2);
}

void UiDemo::buildViews(){
    // test1();
    // test2();
    // test3();
    // test4();
    test5();
}

void UiDemo::tick(){
    rootView_->onRender(renderEngine_); //UI render
}

void UiDemo::dispose(){
    Logi("UiDemo" , "dispose");
}

bool UiDemo::onEventAction(int action , float x , float y){
    // Logi("UiDemo" , "onEventAction event: %d ( %f , %f)" , action , x , y);
    bool cost = rootView_->dispatchTouchEvent(action , x , y);

    if(!cost){
        //todo your owen logic
    }
    return cost;
}

void UiDemo::onClick(View *view){
    std::cout << view->tag_ << " clicked!!!!" << std::endl;
}


