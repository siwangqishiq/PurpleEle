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

     showTextContent = AssetManager::getInstance()->readTextFile("honglou.txt");
//    showTextContent = AssetManager::getInstance()->readTextFile("shader/render_text_frag.glsl");
    Logi("application" , "text file size : %d" , showTextContent.length());

    testShader = ShaderManager::getInstance()->loadAssetShader("test_shader1"
            , "shader/shader_vert.glsl" 
            , "shader/shader_frag.glsl");

    testShader2 = ShaderManager::getInstance()->loadAssetShader("test_shader2"
            , "shader/shader_vert.glsl" 
            , "shader/shader2_frag.glsl");

    startTime = static_cast<long>(currentTimeMillis());
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

    // float x = 0.0f;
    // float y = 0.0f;

    // TextPaint paint1;
    // paint1.textSizeScale = 1.5f;
    // paint1.textColor = glm::vec4(1.0f , 1.0f , 0.0f , 1.0f);
    // renderEngine_->renderText(L"你好世界HelloWorld" , x_ , y_ , paint1);

    // float velocity = 0.0001f;
    // float deltaTime = getLastFrameDeltaTime();
    // x_ += velocity * deltaTime;
    // if(x_ > viewWidth_){
    //     x_ = 0.0f;
    //     y_ += 20.0f;
    // }

    // TextPaint p1;
    // p1.textColor = glm::vec4(0.0f ,1.0f , 0.0f , 1.0f);
    // p1.textSizeScale = 0.5f;
    // Rect limitRect;
    // limitRect.left = 0.0f;
    // limitRect.top = screenHeight_;
    // limitRect.width = screenWidth_ /1.0f;
    // limitRect.height = screenHeight_;
    // std::wstring showStr = showTextContent.substr(0 , showTextContent.length());
    // renderEngine_->renderText(showStr,limitRect, p1);
    
    // textIndex++;
    // if(textIndex >= showTextContent.length()){
    //     textIndex = 0;
    // }

//     for(int i = 0 ; i < 10 ; i++){
//         renderEngine_->renderText(showTextContent , 0, screenHeight_ - i * CHAR_DEFAULT_HEIGHT , p1);
//     }

//    TextPaint p2;
//    p2.textColor = glm::vec4(1.0f ,1.0f , 0.0f , 1.0f);
//    p2.textSizeScale = 1.0f;
//    Rect limitRect2;
//    limitRect2.left = 0.0f;
//    limitRect2.top = screenHeight_;
//    limitRect2.width = screenWidth_ / 3.0f;
//    limitRect2.height = screenHeight_ / 2.0f;
    // renderEngine_->renderText(str,limitRect2, p2);

    // TextPaint p1;
    // p1.textColor = glm::vec4(1.0f ,0.0f , 0.0f , 1.0f);
    // renderEngine_->renderText(L"堆枕乌云堕翠翘" , 100, 300 + 100 + 100, p1);

    // TextPaint p2;
    // p2.textColor = glm::vec4(0.0f ,1.0f , 0.0f , 1.0f);
    // renderEngine_->renderText(L"那更春来,玉减香消" , 100, 300 + 100 , p2);

//    TextPaint p3;
//    p3.textColor = glm::vec4(1.0f ,1.0f , 0.0f , 1.0f);
//    renderEngine_->renderText(L"嬛嬛一袅楚宫腰" , 100, 300 , p3);

    // Rect shaderRect;
    // shaderRect.left = 100.0f;
    // shaderRect.top = screenHeight_;
    // shaderRect.width = 200.0f;
    // shaderRect.height = 200.0f;
    // renderEngine_->renderShader(testShader , shaderRect);

    // Rect shaderRect2;
    // shaderRect2.left = 500.0f;
    // shaderRect2.top = screenHeight_ - 200;
    // shaderRect2.width = 200.0f;
    // shaderRect2.height = 200.0f;
    // renderEngine_->renderShader(testShader2 , shaderRect2);

    // Rect shaderRect2;
    // shaderRect2.left = 300.0f;
    // shaderRect2.top = screenHeight_ - 200;
    // shaderRect2.width = 200.0f;
    // shaderRect2.height = 200.0f;
    Paint paint;

    // long cur = static_cast<long>(currentTimeMillis());
    // long delta = cur - startTime;
    // float t = (float)delta / (float)2000.0f;
    // float colorValueR = glm::sin(t) / 2.0f + 0.5f;
    // float colorValueG = glm::cos(t) / 2.0f + 0.5f;
    // // Logi("time" , "%f" , colorValueR);
    // paint.color = glm::vec4(colorValueR , colorValueG , 1.0f ,1.0f);
    // // paint.color = glm::vec4(1.0f , 1.0f , 0.0f ,1.0f);
    // paint.fillStyle = Stroken;
    // paint.stokenWidth = 100.0f * colorValueR;
    // renderEngine_->renderRect(shaderRect2 , paint);

    // Rect shaderRect3;
    // shaderRect3.left = 0.0f;
    // shaderRect3.top = 200.0f;
    // shaderRect3.width = screenWidth_;
    // shaderRect3.height = 200.0f;
    // renderEngine_->renderRect(shaderRect3 , paint);

    Paint circelPaint;
    circelPaint.fillStyle = Filled;
    circelPaint.stokenWidth = 1.0f;
    circelPaint.color = glm::vec4(1.0f , 0.0f , 1.0f , 1.0f);

    float radius = 32.0;
    float cx = radius;
    float cy = screenHeight_ - radius;

//    renderEngine_->renderCircle(cx , cy , radius , circelPaint);
    long long t1 = currentTimeMicro();

    Logi("viewHeight" , "%f" , (float)viewHeight_ / 4.0f);
    renderEngine_->renderCircle((float)viewWidth_ / 2.0f , (float)viewHeight_ / 2.0f , mRadius , circelPaint);

    mRadius++;
    if(mRadius >= 500.0f){
        mRadius = 100.0f;
    }
//    for(;cy >= 0; cy -= 2*radius){
//        cx = radius;
//        for(;cx <= screenWidth_ ; cx += 2*radius){
//            renderEngine_->renderCircle(cx , cy , radius , circelPaint);
//        }
//    }//end for y
    long long t2 = currentTimeMicro();
    Logi("renderCircle" , "draw circle time: %lld", (t2 - t1));
    
    if(showNumber){
        TextPaint p4;
        p4.textColor = glm::vec4(1.0f ,0.0f , 0.0f , 1.0f);
        std::wstring name = L"帧率:";
        renderEngine_->renderText(name + std::to_wstring(showFps) , screenWidth_ - 220.0f, 
            screenHeight_ - 80.0f, p4);
    }
}

long long Application::getLastFrameDeltaTime(){
    if(timeStamp_ <= 0 ){
        return 0L;
    }
    return currentTimeMicro() - timeStamp_;
}






