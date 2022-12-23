#include "application.hpp"
#include "log.hpp"

//window surface header
#include "glheader.hpp"
#include "render/render.hpp"
#include "resource/asset_manager.hpp"
#include "render/texture.hpp"
#include "render/common.hpp"
#include "widget/timer.hpp"
#include "render/render_batch.hpp"
#include "utils.hpp"

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
    onTick();
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


    // for(int i= 0 ; i < 20 ; i++){
    //     Logi("random" , "%d" , GenRandomInt(0,10));
    // }
    // for(int i= 0 ; i < 20 ; i++){
    //     Logi("random" , "%f" , GenRandomFloat());
    // }
}

void Application::onTick(){
    // Log(TAG , "app trick");
    // Logi(TAG , "getLastFrameDeltaTimeMirco = %lld" , getLastFrameDeltaTime());
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    // testRender1();
//    testRender2();
//    testRender3();
    // testRender4();
//    testRenderRoundRect();
    testRenderTableTennis();

    if(showNumber){
        TextPaint p4;
        p4.textColor = glm::vec4(1.0f ,0.0f , 0.0f , 1.0f);
        std::wstring name = L"帧率:";
        renderEngine_->renderText(name + std::to_wstring(showFps) , screenWidth_ - 220.0f, 
            screenHeight_ - 80.0f, p4);
    }
}

void Application::testRenderTableTennis(){
    // Logi("test" , "viewWidth = %d , viewHeight = %d    screen %d , %d", viewWidth_ , viewHeight_ ,screenWidth_  ,screenHeight_);

    Paint bottomPaint;
    bottomPaint.fillStyle = Filled;
    bottomPaint.color = ConvertColor(210,180,140,255);
    Rect bottomRect;
    bottomRect.height = 0.8f * viewHeight_;
    bottomRect.width = 2 * bottomRect.height;
    bottomRect.left = viewWidth_ / 2.0f - bottomRect.width / 2.0f;
    bottomRect.top = viewHeight_ / 2.0f + bottomRect.height / 2.0f;
    float roundRadius = bottomRect.height / 15.0f;

    Paint groundPaint;
    groundPaint.fillStyle = Filled;
    groundPaint.color = ConvertColor(0,255,0,255);
    Rect groundRect;
    float paddingHor = 60.0f;
    float paddingVtl = 45.0f;
    groundRect.width = bottomRect.width - 2 * paddingHor;
    groundRect.height = bottomRect.height - 2 * paddingVtl;
    groundRect.left = viewWidth_ / 2.0f - groundRect.width / 2.0f;
    groundRect.top = viewHeight_ / 2.0f + groundRect.height / 2.0f;

    //hole
    Paint holePaint;
    holePaint.fillStyle = Filled;
    holePaint.color = ConvertColor(0 , 0, 0, 255);

    float holeRadius = 30.0f;
    glm::vec2 holePosition[6];
    holePosition[0].x = groundRect.left;
    holePosition[0].y = groundRect.top;

    holePosition[1].x = groundRect.left + groundRect.width / 2.0f;
    holePosition[1].y = groundRect.top;

    holePosition[2].x = groundRect.getRight();
    holePosition[2].y = groundRect.top;

    holePosition[3].x = groundRect.left;
    holePosition[3].y = groundRect.getBottom();

    holePosition[4].x = groundRect.left + groundRect.width / 2.0f;
    holePosition[4].y = groundRect.getBottom();

    holePosition[5].x = groundRect.getRight();
    holePosition[5].y = groundRect.getBottom();

    renderEngine_->getShapeBatch()->begin();
    renderEngine_->getShapeBatch()->renderRoundRect(bottomRect , roundRadius ,bottomPaint);
    renderEngine_->getShapeBatch()->renderRect(groundRect , groundPaint);

    for(int i = 0 ; i < 6 ;i++){
        auto pos = holePosition[i];
        renderEngine_->getShapeBatch()->renderCircle(pos.x , pos.y , holeRadius , holePaint);
    }//end for i

    renderEngine_->getShapeBatch()->end();
}

void Application::testRenderRoundRect(){
    Paint paint;
    paint.fillStyle = Filled;
    paint.stokenWidth = 1.0f;
    paint.color = glm::vec4(0.0f , 1.0f , 0.0f , 1.0f);

    float width = 300.0f;
    float height = 200.0f;
    float left = screenWidth_ / 2.0f - width / 2.0f;
    float top = screenHeight_ / 2.0f + height / 2.0f;
    float radius = mRadius++;

    if(mRadius >= height /2.0f){
        mRadius = 1.0f;
    }

    renderEngine_->getShapeBatch()->begin();
    Rect rect;
    rect.left = left;
    rect.top = top;
    rect.width = width;
    rect.height = height;
    renderEngine_->getShapeBatch()->renderRoundRect(rect , radius , paint);
    renderEngine_->getShapeBatch()->end();
}

void Application::testRender4(){
    bool isFilled = false;
    Paint circelPaint;
    circelPaint.fillStyle = Filled;
    circelPaint.stokenWidth = 1.0f;
    circelPaint.color = glm::vec4(0.0f , 1.0f , 0.0f , 1.0f);
    renderEngine_->getShapeBatch()->begin();

    float width = 50.0f;
    float height = 40.0f;
    float x = 0.0f;
    float y = 0.0f;
    float padding = 4.0f;
    int rectCount = 0;

    long long t1 = currentTimeMicro();
    for(; y <= screenHeight_+ height; y += height + padding){
        for(x = 0.0f;x <= screenWidth_ ; x += width + padding){
            Rect rect;
            rect.left = x;
            rect.top = y;
            rect.width = width;
            rect.height = height;
            rectCount++;
            renderEngine_->getShapeBatch()->renderOval(rect , circelPaint);
        }//end for x
    }//end for y
    renderEngine_->getShapeBatch()->end();
    long long t2 = currentTimeMicro();
    // Logi("test_render" , "delta %lld count : %d" , (t2 - t1) , rectCount);
}

void Application::testRender3(){
    Paint circelPaint;
    circelPaint.fillStyle = Stroken;
    circelPaint.stokenWidth = 1.0f;
    circelPaint.color = glm::vec4(0.0f , 1.0f , 0.0f , 1.0f);

    mRadius++;

    float cx = screenWidth_ / 2.0f;
    float cy = screenHeight_ / 2.0f;
    renderEngine_->getShapeBatch()->begin();

    float width = 32.0f;
    float height = 32.0f;
    float x = 0.0f;
    float y = height;
    float padding = 2.0f;
    int rectCount = 0;
    long long t1 = currentTimeMicro();
    for(; y <= screenHeight_ ; y += height + padding){
        for(x = 0.0f;x <= screenWidth_ ; x += width + padding){
            Rect rect;
            rect.left = x;
            rect.top = y;
            rect.width = width;
            rect.height = height;
            rectCount++;
            Paint paint;
            paint.fillStyle = Stroken;
            paint.stokenWidth = 1.0f;
            paint.color = glm::vec4(0.0f , 1.0f , 1.0f , 1.0f);
            renderEngine_->getShapeBatch()->renderRect(rect , paint);
        }//end for x
    }//end for y

    renderEngine_->getShapeBatch()->renderCircle(cx , cy , mRadius , circelPaint);
    renderEngine_->getShapeBatch()->end();
    long long t2 = currentTimeMicro();

    if(mRadius > screenHeight_ / 2.0f){
        mRadius = 0.0f;
    }
}

void Application::testRender2(){
    Paint circelPaint;
    circelPaint.fillStyle = Filled;
    circelPaint.stokenWidth = 1.0f;
    circelPaint.color = glm::vec4(0.0f , 1.0f , 0.0f , 1.0f);

    float radius = 16.0f;
    float cx = radius;
    float cy = screenHeight_ - radius;
    float padding = 4.0f;

    bool isFilled = false;
    renderEngine_->getShapeBatch()->begin();
    for(;cy >= 0; cy -= 2*radius + padding){
        cx = radius;
        for(;cx <= screenWidth_ ; cx += 2*radius + padding){
            circelPaint.fillStyle = GenRandomFloat()>0.5f?Filled:Stroken;
            // renderEngine_->renderCircle(cx , cy , radius , circelPaint);
            circelPaint.color = glm::vec4(GenRandomFloat() , GenRandomFloat() , GenRandomFloat() , 1.0f);
            renderEngine_->getShapeBatch()->renderCircle(cx , cy , radius , circelPaint);
            isFilled = !isFilled;
        }
    }//end for y
    renderEngine_->getShapeBatch()->end();
}

void Application::testRender1(){
    bool isFilled = false;
    Paint circelPaint;
    circelPaint.fillStyle = Stroken;
    circelPaint.stokenWidth = 1.0f;
    circelPaint.color = glm::vec4(0.0f , 1.0f , 0.0f , 1.0f);
    renderEngine_->getShapeBatch()->begin();

    float width = 32.0f;
    float height = 32.0f;
    float x = 0.0f;
    float y = height;
    float padding = 2.0f;
    int rectCount = 0;

    long long t1 = currentTimeMicro();
    for(; y <= screenHeight_ ; y += height + padding){
        for(x = 0.0f;x <= screenWidth_ ; x += width + padding){
            Rect rect;
            rect.left = x;
            rect.top = y;
            rect.width = width;
            rect.height = height;
            rectCount++;
               
//             circelPaint.fillStyle = GenRandomFloat() >0.5f?Filled:Stroken;
//            circelPaint.color = glm::vec4(GenRandomFloat() , GenRandomFloat() , GenRandomFloat() , 1.0f);
            renderEngine_->getShapeBatch()->renderRect(rect , circelPaint);

            isFilled = !isFilled;
        }//end for x
    }//end for y
    renderEngine_->getShapeBatch()->end();
    long long t2 = currentTimeMicro();
    // Logi("test_render" , "delta %lld count : %d" , (t2 - t1) , rectCount);
}

long long Application::getLastFrameDeltaTime(){
    // if(timeStamp_ <= 0 ){
    //     return 0L;
    // }
    return currentTimeMicro() - timeStamp_;
}






