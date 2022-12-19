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
    testRender1();
    
    if(showNumber){
        TextPaint p4;
        p4.textColor = glm::vec4(1.0f ,0.0f , 0.0f , 1.0f);
        std::wstring name = L"帧率:";
        renderEngine_->renderText(name + std::to_wstring(showFps) , screenWidth_ - 220.0f, 
            screenHeight_ - 80.0f, p4);
    }
}

void Application::testRender1(){
    Paint circelPaint;
    circelPaint.fillStyle = Stroken;
    circelPaint.stokenWidth = 1.0f;
    circelPaint.color = glm::vec4(1.0f , 1.0f , 0.0f , 1.0f);
    
    float radius = 8.0f;

    float cx = radius;
    float cy = screenHeight_ - radius;

    bool isFilled = false;
    for(;cy >= 0; cy -= 2*radius){
        cx = radius;
        for(;cx <= screenWidth_ ; cx += 2*radius){
            // circelPaint.fillStyle = isFilled?Filled:Stroken;
            renderEngine_->renderCircle(cx , cy , radius , circelPaint);
            // isFilled = !isFilled;
        }
    }//end for y
    long long t2 = currentTimeMicro();
}

long long Application::getLastFrameDeltaTime(){
    if(timeStamp_ <= 0 ){
        return 0L;
    }
    return currentTimeMicro() - timeStamp_;
}






