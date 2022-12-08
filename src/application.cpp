#include "application.hpp"
#include "log.hpp"

//window surface header
#include "glheader.hpp"
#include "render/render.hpp"
#include "resource/asset_manager.hpp"
#include "render/texture.hpp"
#include "render/common.hpp"
#include "widget/timer.hpp"

void Application::onFree(){
    Logi(TAG , "app onFree");
    if(timer_ != nullptr){
        timer_->clear();
    }
    // triangleDemo_->free();
    if(renderEngine_ != nullptr){
        renderEngine_->free();
    }
}   

void Application::init(){
    onInit();
}


void Application::update(){
    onTrick();
}

void Application::free(){
    onFree();
}

Application::~Application(){
    Logi(TAG , "~app Application");
}

void Application::onResize(int w , int h){
    Logi(TAG , "app onresize %d , %d" , w , h);
    screenWidth_ = w;
    screenHeight_ = h;
    
    if(renderEngine_ != nullptr){
        renderEngine_->onScreenResize();
    }
}

std::shared_ptr<Timer> Application::getTimer(){
    if(timer_ == nullptr){
        timer_ = std::make_shared<Timer>();
    }

    return timer_;
}

void Application::onInit(){
    Logi(TAG , "app onInit");

    // renderEngine_ = std::shared_ptr<RenderEngine>(new RenderEngine(std::shared_ptr<Application>(this)));

    renderEngine_ = std::make_shared<RenderEngine>(this);
    renderEngine_->init();
    
    renderEngine_->onScreenResize();

    triangleDemo_ = std::make_shared<Triangle>();
    triangleDemo_->init();

    // getTimer()->schedule([this](Application *app){
    //     Logi("timer" , "hello timer1111!");
    // } , 20 * 1000L);


    // fixedRateTaskId = getTimer()->scheduleAtFixedRate([this](Application *app){
    //     Logi("timer" , "fixed %lld" , currentTimeMillis());
    //     app->mIndex++;
    //     int rmId = app->fixedRateTaskId;
    //     if(app->mIndex >= 10){
    //         app->getTimer()->removeScheduleTask(rmId);
    //     }
    // } , 1000L);



    showNumber = true;

    // TextPaint paint;
    // renderEngine_->renderText(std::to_wstring(mIndex_++) , 0 , 0 , paint);
    
    // auto fileContent = AssetManager::getInstance()->readTextFile("test.txt");
    // Logi("asset" , "test.txt szie: %d \n content: %s" , 
    //         fileContent.size() , fileContent.c_str());

    // TextureFileConfig config;
    // AssetManager::getInstance()->readTextureFile("lan.jpg", config);
    // Logi("asset" , "image info width : %d , height %d , channel : %d , datasize : %d",
    //      config.width , config.height , config.channel , config.dataSize);
    
    // auto info1 = TextureManager::getInstance()->acquireTexture("text/font_texture_0.png");
    // auto info2 = TextureManager::getInstance()->acquireTexture("text/font_texture_1.png");

    timeStamp_ = currentTimeMicro();

    onCreate();
}

void Application::onCreate(){
    getTimer()->scheduleAtFixedRate([this](Application *app){
        Logi("application" , "fps : %d" , frameCount_);
        showFps = frameCount_;
        frameCount_ = 0;
    } , 1000L);
}

void Application::onTrick(){
    // Log(TAG , "app trick");
    // Logi(TAG , "getLastFrameDeltaTimeMirco = %lld" , getLastFrameDeltaTime());

    long long timeStart = currentTimeMillis();
    if(renderEngine_ == nullptr){
        return;
    }
    
    //user logic and draw
    updateSence();

    //定时器triker
    if(timer_ != nullptr){
        timer_->trick(this);
    }

    //gl commands run
    renderEngine_->render();
    
    // for test
    // triangleDemo_->trick(renderEngine_->normalMatrix_);

    timeStamp_ = currentTimeMicro();
    long timeEnd = currentTimeMillis();
    auto deltaTime = timeEnd - timeStart;
    frameCount_++;

    // Logi(TAG , "frame cost time : %ld" , deltaTime);
}

void Application::updateSence(){
    // float x = x_;

    float x = 0.0f;
    float y = 0.0f;

    TextPaint paint1;
    paint1.textSizeScale = 1.5f;
    paint1.textColor = glm::vec4(1.0f , 1.0f , 0.0f , 1.0f);
    // paint1.textStyle = TextStyle::italic;
    renderEngine_->renderText(L"你好世界HelloWorld" , x_ , y_ , paint1);
    float velocity = 0.0001f;
    float deltaTime = getLastFrameDeltaTime();
    x_ += velocity * deltaTime;
    if(x_ > viewWidth_){
        x_ = 0.0f;
        y_ += 20.0f;
    }

    TextPaint p1;
    p1.textColor = glm::vec4(1.0f ,1.0f , 1.0f , 1.0f);
    p1.textSizeScale = 1.0f;
    Rect limitRect;
    limitRect.left = 0.0f;
    limitRect.top = screenHeight_;
    limitRect.width = screenWidth_;
    limitRect.height = screenHeight_;
    std::wstring str = L"  多年以后，奥雷连诺上校站在行刑队面前，准会想起父亲带他去参观冰块的那个遥远的下午。当时，马孔多是个二十户人家的村庄，一座座土房都盖在河岸上，河水清澈，沿着遍布石头的河床流去，河里的石头光滑、洁白，活象史前的巨蛋。这块天地还是新开辟的，许多东西都叫不出名字，不得不用手指指点点。每年三月，衣衫褴楼的吉卜赛人都要在村边搭起帐篷，在笛鼓的喧嚣声中，向马孔多的居民介绍科学家的最新发明。他们首先带来的是磁铁。一个身躯高大的吉卜赛人，自称梅尔加德斯，满脸络腮胡子，手指瘦得象鸟的爪子，向观众出色地表演了他所谓的马其顿炼金术士创造的世界第八奇迹。他手里拿着两大块磁铁，从一座农舍走到另一座农舍，大家都惊异地看见，铁锅、铁盆、铁钳、铁炉都从原地倒下，木板上的钉子和螺丝嘎吱嘎吱地拼命想挣脱出来，甚至那些早就丢失的东西也从找过多次的地方兀然出现，乱七八糟地跟在梅尔加德斯的魔铁后面。“东西也是有生命的，”吉卜赛人用刺耳的声调说，“只消唤起它们的灵性。”霍·阿·布恩蒂亚狂热的想象力经常超过大自然的创造力，甚至越过奇迹和魔力的限度，他认为这种暂时无用的科学发明可以用来开采地下的金子";

    std::wstring showStr = str.substr(0 , textIndex);
    renderEngine_->renderText(showStr,limitRect, p1);

    textIndex++;
    if(textIndex >= str.length()){
        textIndex = 0;
    }

    // for(int i = 0 ; i < 10 ; i++){
    //     renderEngine_->renderText(str , 0, screenHeight_ - i * CHAR_DEFAULT_HEIGHT , p1);
    // }

    TextPaint p2;
    p2.textColor = glm::vec4(1.0f ,1.0f , 0.0f , 1.0f);
    p2.textSizeScale = 1.0f;
    Rect limitRect2;
    limitRect2.left = 0.0f;
    limitRect2.top = screenHeight_;
    limitRect2.width = screenWidth_ / 3.0f;
    limitRect2.height = screenHeight_ / 2.0f;
    // renderEngine_->renderText(str,limitRect2, p2);

    // TextPaint p1;
    // p1.textColor = glm::vec4(1.0f ,0.0f , 0.0f , 1.0f);
    // renderEngine_->renderText(L"堆枕乌云堕翠翘" , 100, 300 + 100 + 100, p1);

    // TextPaint p2;
    // p2.textColor = glm::vec4(0.0f ,1.0f , 0.0f , 1.0f);
    // renderEngine_->renderText(L"那更春来,玉减香消" , 100, 300 + 100 , p2);

    // TextPaint p3;
    // p3.textColor = glm::vec4(1.0f ,1.0f , 0.0f , 1.0f);
    // p3.textStyle = TextStyle::italic;
    // renderEngine_->renderText(L"嬛嬛一袅楚宫腰" , 100, 300 , p3);

    if(showNumber){
        TextPaint p4;
        p4.textSizeScale = 1.0f;
        p4.textColor = glm::vec4(1.0f ,0.0f , 0.0f , 1.0f);
        renderEngine_->renderText(std::to_wstring(showFps) , 0, 0 , p4);
    }
}

long long Application::getLastFrameDeltaTime(){
    if(timeStamp_ <= 0 ){
        return 0L;
    }
    return currentTimeMicro() - timeStamp_;
}






