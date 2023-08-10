#include "test_demo.hpp"
#include "utils.hpp"
#include "render/common.hpp"
#include "render/render.hpp"
#include "render/render_batch.hpp"
#include "audio/audio.hpp"
#include "render/sprite.hpp"
#include <cmath>
#include "widget/timer.hpp"
#include "resource/asset_manager.hpp"
#include "input/input_manager.hpp"

void TestDemo::init(){
    viewWidth_ = appContext->viewWidth_;
    viewHeight_ = appContext->viewHeight_;

    renderEngine_ = appContext->getRender();
        
    appContext->addEventActionCallback(this);

    // testAudio();
    testImage = BuildImageByAsset("lan.jpg");
    testGakkiImage = BuildImageByAsset("test.jpeg");
    wallpaperImage = BuildImageByAsset("bg.jpg");
    walkingImage = BuildImageByAsset("sprite/walk.png");//32 x 48
    yuanImage_ = BuildImageByAsset("sprite/yuan.png");
    customRenderShader = ShaderManager::getInstance()->loadAssetShader("custom" ,
                                                                       "shader/shader_vert.glsl" ,
                                                                        "shader/shader_frag.glsl");
    
    Logi("testDemo" , "testImage w : %d , h : %d" , 
        testImage->getWidth(),testImage->getHeight());

    if(isInited){
        return;
    }
    isInited = true;
    appContext->getTimer()->scheduleAtFixedRate([this](Application *app){
        walkIndex++;
        if(walkIndex >= 4){
            walkIndex = 0;
        }
    } , 100L);

    //some gl config message debug
    GLint size = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE , &size);
    Logi("testDemo" , "max texture size = %d"  ,size);
    GLint max3dTextureSize = 0;
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE , &max3dTextureSize);
    Logi("testDemo" , "max 3d texture size = %d"  ,max3dTextureSize);

    int maxTextureImageUnits[1];
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, maxTextureImageUnits);
    Logi("testDemo" , "max texture image units = %d"  ,maxTextureImageUnits[0]);

    code_ = AssetManager::getInstance()->readTextFile("teng.txt");
    Logi("debug" , "read code size %d" , code_.size());

    testLoadAudioFile();
}

void TestDemo::tick(){
//     testRender1();
//     testRender2();
//     testRender3();
//     testRender4();
//     testRenderRoundRect();
//     testRenderTableTennis();
//     testRenderSprite1();
//     testRenderSprite2();
//     testRenderSprite3();
//     testRenderSprite4();
//     testRenderSprite5Rotate();
   //  testRenderSprite6();
    // testRenderSprite6ImageRegion();

    // testRenderShader();

//     testRenderText();
//     testRenderTextWithRect();
    // testRenderTextGravity();
//     testRenderTextCode();

    // testActionDown();
    //  testRenderBlurCircle();
    // testRenderLinearGradRect();
//     testRenderBlurRect();
//     testRenderBlurRect2();
//     testRenderBlurRect3();
    // testRenderBlurRect4();
    // testRenderBlurRoundRect();
     testRenderBlurRoundRect2();
}

void TestDemo::testLoadAudioFile(){
//    music_ = AudioManager::getInstance()->loadAudioEntity("audio/pao.mp3" , true);
//    audio_ = AudioManager::getInstance()->loadAudioEntity("audio/click1.wav");
//
//    AudioManager::getInstance()->playAudioEntity(music_);

    AudioManager::getInstance()->loadAudio("audio/click1.wav","click1");
    AudioManager::getInstance()->loadAudio("audio/click2.wav","click2");
    AudioManager::getInstance()->loadAudio("audio/click3.wav","click3");

    AudioManager::getInstance()->loadAudio("audio/pao.mp3","bai");

    AudioManager::getInstance()->playAudio("bai");
}

void TestDemo::dispose(){
//    AudioManager::getInstance()->releaseAudioEntity(audio_);
//    AudioManager::getInstance()->releaseAudioEntity(music_);
}

bool TestDemo::onEventAction(int action , float x , float y){
    Logi("testdemo" , "onEventAction event: %d ( %f , %f)" , action , x , y);
    if(action == ACTION_UP){
        count_++;

        // if(AudioManager::getInstance()->isPlayAudioEntity(audio_)){
        //     AudioManager::getInstance()->stopAudioEntity(audio_);
        // }else{
        //     AudioManager::getInstance()->playAudioEntity(audio_);
        // }
        // AudioManager::getInstance()->playAudioEntity(audio_);
        // AudioManager::getInstance()->restartAudioEntity(music_);
    }else if(action == ACTION_DOWN){
//        AudioManager::getInstance()->playAudioEntity(audio_);

        if(x < viewWidth_ / 3.0f){
            AudioManager::getInstance()->playAudio("click1");
        }else if(x >  viewWidth_ / 3.0f && x < (2.0f * viewWidth_) / 3.0f){
            AudioManager::getInstance()->playAudio("click2");
        }else{
            AudioManager::getInstance()->playAudio("click3");
        }
    }
    return false;
}

void TestDemo::testRenderBlurRoundRect2(){
     Paint paint;
    paint.color = glm::vec4(0.0f , 0.0f , 1.0f , 1.0f);

    float blurSize = 15.0f * glm::sin(deltatime_) + 15.0f;

    deltatime_ += 0.05f;

    renderEngine_->getShapeBatch()->begin();

    float cubeSize = 100.0f;
    float gapSize = 15.0f * 2.0f;

    float x = 0.0f;
    float y = viewHeight_;

    for(;y > 0.0f ; y -= cubeSize + gapSize){
        for(x = 0.0f ; x < viewWidth_; x += cubeSize + gapSize){

            Rect rect;
            rect.left = x;
            rect.top = y;
            rect.width = cubeSize;
            rect.height = cubeSize;

            paint.color = glm::vec4(GenRandomFloat() , GenRandomFloat() , GenRandomFloat() , 1.0f);

            renderEngine_->getShapeBatch()
                ->renderBlurRoundRect(rect ,8.0f, blurSize , paint);
        }
    }//end for y
    
    renderEngine_->getShapeBatch()->end();
}

void TestDemo::testRenderBlurRoundRect(){
    Paint bottomPaint;
    bottomPaint.fillStyle = Filled;
    bottomPaint.color = glm::vec4(1.0f , 1.0f , 1.0f ,1.0f);

    Rect bottomRect;
    bottomRect.left = (viewWidth_ - viewHeight_)/2.0f;
    bottomRect.top = viewHeight_;
    bottomRect.height = viewWidth_;
    bottomRect.width = viewHeight_;

    renderEngine_->getShapeBatch()->begin();
    renderEngine_->getShapeBatch()->renderRect(bottomRect , bottomPaint);

    Rect rect;
    rect.width = 300.0f;
    rect.height = 200.0f;
    rect.left = viewWidth_ / 2.0 - rect.width / 2.0f;
    rect.top = viewHeight_ / 2.0 + rect.height / 2.0f;

    Paint paint;
    paint.color = COLOR_SKY_BLUE;
    paint.fillStyle = Filled; 

    float blur = 30.0f * glm::sin(deltatime_) + 30.0f;
    deltatime_ += 0.01f;
    renderEngine_->getShapeBatch()->renderBlurRoundRect(rect , 50.0f , blur , paint);
    renderEngine_->getShapeBatch()->end();    
}

void TestDemo::testActionDown() {
    TextPaint paint;
    float fontSize = 128.0f;
    paint.setTextSize(fontSize);
    paint.textColor = glm::vec4(1.0 ,1.0 ,1.0, 1.0f);
    paint.textGravity = Center;

    Rect showRect;
    showRect.width = viewWidth_;
    showRect.height = viewHeight_;
    showRect.top = viewHeight_;
    showRect.left = 0;

    std::wstring countStr = L"";
    auto countS = std::to_string(count_);
    countStr += std::wstring(countS.begin() , countS.end());

    renderEngine_->renderTextWithRect(countStr , showRect , paint , nullptr);
}

void TestDemo::testRenderTextCode(){
    auto batch = renderEngine_->getSpriteBatch();
    Rect srcRect;
    srcRect.left = 0.0;
    srcRect.top = wallpaperImage->getHeight();
    srcRect.width = wallpaperImage->getWidth();
    srcRect.height = wallpaperImage->getHeight();
    Rect dstRect;
    dstRect.left = 0;
    dstRect.top = viewHeight_;
    dstRect.width = viewWidth_;
    dstRect.height = viewHeight_;

    batch->begin();
    batch->renderImage(wallpaperImage, srcRect , dstRect);
    batch->end();

    Rect limitRect;
    limitRect.left = 0.0;
    limitRect.top = viewHeight_;
    limitRect.width = viewWidth_;
    limitRect.height = viewHeight_;

    TextPaint paint;
    float fontSize = 64.0f;
    paint.setTextSize(fontSize);
    paint.textColor = glm::vec4(1.0 ,0.0 ,0.0, 1.0f);
    paint.textGravity = Center;

    Rect warpRect;

    renderEngine_->renderTextWithRect(code_ ,
        limitRect ,
        paint ,
        &warpRect);

    Paint warpPaint;
    warpPaint.color = glm::vec4(1.0f , 0.0f ,0.0f ,0.3f);
    auto shapeBatch = renderEngine_->getShapeBatch();
    shapeBatch->begin();
    shapeBatch->renderRect(warpRect , warpPaint);
    shapeBatch->end();
}

void TestDemo::testRenderTextGravity(){
    auto batch = renderEngine_->getSpriteBatch();
    Rect srcRect;
    srcRect.left = 0.0;
    srcRect.top = wallpaperImage->getHeight();
    srcRect.width = wallpaperImage->getWidth();
    srcRect.height = wallpaperImage->getHeight();
    Rect dstRect;
    dstRect.left = 0;
    dstRect.top = viewHeight_;
    dstRect.width = viewWidth_;
    dstRect.height = viewHeight_;

    batch->begin();
    batch->renderImage(wallpaperImage, srcRect , dstRect);
    batch->end();


    Rect limitRect;
    limitRect.left = 0.0;
    limitRect.top = viewHeight_;
    limitRect.width = viewWidth_;
    limitRect.height = viewHeight_;

    TextPaint paint;
    float fontSize = 64.0f;
    paint.setTextSize(fontSize);
    paint.textColor = glm::vec4(1.0 ,1.0 ,1.0, 1.0f);
    paint.textGravity = TopLeft;

    renderEngine_->renderTextWithRect(L"你好世界1" ,
        limitRect ,
        paint ,
        nullptr);

    paint.textGravity = TopCenter;
    renderEngine_->renderTextWithRect(L"你好世界2" ,
        limitRect ,
        paint ,
        nullptr);

    paint.textGravity = TopRight;
    renderEngine_->renderTextWithRect(L"你好世界3" ,
        limitRect ,
        paint ,
        nullptr);

    paint.textGravity = BottomLeft;
    renderEngine_->renderTextWithRect(L"你好世界4" ,
        limitRect ,
        paint ,
        nullptr);

    paint.textGravity = BottomCenter;
    renderEngine_->renderTextWithRect(L"你好世界5" ,
        limitRect ,
        paint ,
        nullptr);

    paint.textGravity = BottomRight;
    renderEngine_->renderTextWithRect(L"你好世界6" ,
        limitRect ,
        paint ,
        nullptr);

    paint.textGravity = CenterLeft;
    paint.textColor = glm::vec4(1.0f , 0.0f ,0.0f ,1.0f);
    renderEngine_->renderTextWithRect(L"你好世界7" ,
        limitRect ,
        paint ,
        nullptr);

    paint.textGravity = CenterRight;
    paint.textColor = glm::vec4(1.0f , 1.0f ,0.0f ,1.0f);
    renderEngine_->renderTextWithRect(L"你好世界8" ,
        limitRect ,
        paint ,
        nullptr);

    paint.textGravity = Center;
    paint.textColor = glm::vec4(0.0f , 1.0f ,0.0f ,1.0f);
    renderEngine_->renderTextWithRect(L"惨" ,
        limitRect ,
        paint ,
        nullptr);
}

void TestDemo::testRenderTextWithRect(){
    auto batch = renderEngine_->getSpriteBatch();
    Rect srcRect;
    srcRect.left = 0.0;
    srcRect.top = wallpaperImage->getHeight();
    srcRect.width = wallpaperImage->getWidth();
    srcRect.height = wallpaperImage->getHeight();
    Rect dstRect;
    dstRect.left = 0;
    dstRect.top = viewHeight_;
    dstRect.width = viewWidth_;
    dstRect.height = viewHeight_;

    batch->begin();
    batch->renderImage(wallpaperImage, srcRect , dstRect);
    batch->end();


    Rect limitRect;
    limitRect.left = 0.0;
    limitRect.top = viewHeight_;
    limitRect.width = viewWidth_ / 2.0f;
    limitRect.height = viewHeight_;

    TextPaint paint;
    static float fSize= 32.0f;
    fSize += 0.1f;
    if(fSize >= 128.0){
        fSize = 32.0f;
    }
    float fontSize = fSize;
    paint.setTextSize(fontSize);
    paint.textColor = glm::vec4(1.0 ,1.0 ,1.0, 1.0f);
    paint.textGravity = BottomRight;

    Rect outRect;
    renderEngine_->renderTextWithRect(L"你好世界 \n一二三四五六 \tThank you! \n哈哈哈哈" ,
        limitRect ,
        paint ,
        &outRect);

    auto shapeBatch = renderEngine_->getShapeBatch();
    shapeBatch->begin();
    Paint limitRectPaint;
    limitRectPaint.color = glm::vec4(1.0 , 0.0 , 1.0f , 1.0f);
    limitRectPaint.stokenWidth = 4.0f;
    limitRectPaint.fillStyle = Stroken; 
    shapeBatch->renderRect(limitRect , limitRectPaint);

    Paint rectPaint;
    rectPaint.color = glm::vec4(0.0 , 0.0 , 1.0f , 0.2f);
    shapeBatch->renderRect(outRect , rectPaint);
    shapeBatch->end();
}

void TestDemo::testRenderText(){
    auto batch = renderEngine_->getSpriteBatch();
    Rect srcRect;
    srcRect.left = 0.0;
    srcRect.top = wallpaperImage->getHeight();
    srcRect.width = wallpaperImage->getWidth();
    srcRect.height = wallpaperImage->getHeight();
    Rect dstRect;
    dstRect.left = 0;
    dstRect.top = viewHeight_;
    dstRect.width = viewWidth_;
    dstRect.height = viewHeight_;

    batch->begin();
    batch->renderImage(wallpaperImage, srcRect , dstRect);
    batch->end();

    static float y=0.0;
    TextPaint paint;
    float fontSize = 100.0f;
    paint.setTextSize(fontSize);
    paint.textColor = glm::vec4(0.0 ,0.0 ,0.0 , 1.0f);
    float offsetX = 100.0f;
    renderEngine_->renderText(L"滕王高阁临江渚，佩玉鸣鸾罢歌舞" ,
        offsetX, y , paint);
    renderEngine_->renderText(L"画栋朝飞南浦云,珠帘暮卷西山雨" ,
        offsetX, y - fontSize , paint);
    renderEngine_->renderText(L"闲云潭雨日悠悠,物换星移几度秋" ,
        offsetX, y - 2 * fontSize , paint);
    renderEngine_->renderText(L"阁中帝子今何在？槛外长江空自流" ,
        offsetX, y - 3 * fontSize , paint);
    
    y = (y + 1.0f);
    if(y > viewHeight_){
        y = 0.0f;
    }
}


void TestDemo::testRenderSprite6ImageRegion(){
    auto spriteBatch = renderEngine_->getSpriteBatch();
    spriteBatch->begin();

    Rect dstRect;
    dstRect.width = 160.0f;
    dstRect.height = 240.0f;
    dstRect.left = 0.0f;
    dstRect.top = viewHeight_ / 2.0f + dstRect.height / 2.0f;

    // Logi("testDemo" , "walkIndex = %d" , appContext->walkIndex);
    auto regionImage = walkingImage->createImageRegion(32 * walkIndex , 48 * 4, 32 , 48);
    spriteBatch->renderRegionImage(*regionImage , dstRect);

    Rect dstRect2 = dstRect;
    dstRect2.left = dstRect.getRight();
    dstRect2.top = viewHeight_ / 2.0f + dstRect.height / 2.0f;
    spriteBatch->renderRegionImage(*regionImage , dstRect2);
    spriteBatch->end();
}

void TestDemo::testRenderSprite5Rotate(){
    auto spriteBatch = renderEngine_->getSpriteBatch();
    spriteBatch->begin();

    Rect srcRect;
    srcRect.left = 0.0f;
    srcRect.top = testImage->getHeight();
    srcRect.height = testImage->getHeight();
    srcRect.width = testImage->getWidth();

    Rect dstRect;
    dstRect.width = 300.0f;
    dstRect.height = 300.0f;
    dstRect.left = viewWidth_ / 2.0f - dstRect.width / 2.0f;
    dstRect.top = viewHeight_ / 2.0f + dstRect.height / 2.0f;

    auto center = dstRect.center();
    spriteBatch->renderImage(*testImage , srcRect , dstRect , 
        center.x , center.y , rotateAngle);
    spriteBatch->end();

    rotateAngle -= PI / 180.0f;
}

void TestDemo::testRenderSprite6(){
    auto spriteBatch = renderEngine_->getSpriteBatch();
    spriteBatch->begin();

    Rect srcRect;
    srcRect.left = 0.0f;
    srcRect.top = yuanImage_->getHeight();
    srcRect.height = yuanImage_->getHeight();
    srcRect.width = yuanImage_->getWidth();

    float ratio = static_cast<float>(yuanImage_->getWidth()) / yuanImage_->getHeight();

    Rect dstRect;
    dstRect.height = appContext->viewHeight_;
    dstRect.width = ratio * (dstRect.height);
    dstRect.left = (appContext->viewWidth_ - dstRect.width) / 2.0f;
    dstRect.top = appContext->viewHeight_;
    
    spriteBatch->renderImage(*yuanImage_ , srcRect , dstRect);
    spriteBatch->end();
}

void TestDemo::testRenderSprite1(){
    auto spriteBatch = renderEngine_->getSpriteBatch();
    spriteBatch->begin();
    
    Rect srcRect;
    srcRect.left = 0.0f;
    srcRect.top = testImage->getHeight();
    srcRect.height = testImage->getHeight();
    srcRect.width = testImage->getWidth();

    Rect dstRect;
    dstRect.left = 0.0f;
    dstRect.top = viewHeight_;
    float scale = 1.0f / 2.0f;
    dstRect.width = testImage->getWidth() * scale;
    dstRect.height = testImage->getHeight() * scale;

    spriteBatch->renderImage(*testImage , srcRect , dstRect);
    spriteBatch->end();
}

void TestDemo::testRenderSprite2(){
    auto spriteBatch = renderEngine_->getSpriteBatch();
    spriteBatch->begin();

    Rect srcRect1;
    srcRect1.left = 0.0f;
    srcRect1.top = testGakkiImage->getHeight();
    srcRect1.height = testGakkiImage->getHeight();
    srcRect1.width = testGakkiImage->getWidth();

    float scale = 1.0f / 2.0f;
    Rect dstRect1;
    dstRect1.left = 0.0f;
    dstRect1.top = viewHeight_;
    dstRect1.width = testGakkiImage->getWidth() * scale;
    dstRect1.height = testGakkiImage->getHeight() * scale;

    Rect srcRect2;
    srcRect2.left = 0.0f;
    srcRect2.top = testImage->getHeight();
    srcRect2.height = testImage->getHeight();
    srcRect2.width = testImage->getWidth();

    Rect dstRect2;
    dstRect2.left = dstRect1.getRight();
    dstRect2.top = viewHeight_;
    dstRect2.width = testImage->getWidth() * scale;
    dstRect2.height = testImage->getHeight() * scale;

    spriteBatch->renderImage(*testGakkiImage , srcRect1 , dstRect1);
    spriteBatch->renderImage(*testImage , srcRect2 , dstRect2);

    spriteBatch->end();
}

void TestDemo::testRenderSprite4(){
    auto spriteBatch = renderEngine_->getSpriteBatch();
    Rect srcRect;
    srcRect.left = 0.0f;
    srcRect.top = wallpaperImage->getHeight();
    srcRect.height = wallpaperImage->getHeight();
    srcRect.width = wallpaperImage->getWidth();

    Rect dstRect;
    dstRect.left = 0.0f;
    dstRect.top = viewHeight_;
    dstRect.width = viewWidth_;
    dstRect.height = viewHeight_;

    spriteBatch->begin();
    spriteBatch->renderImage(*wallpaperImage , srcRect , dstRect);
    spriteBatch->end();
}

void TestDemo::testRenderSprite3(){
    std::shared_ptr<SpriteBatch> spriteBatch = renderEngine_->getSpriteBatch();
    spriteBatch->begin();

    auto curImage = testGakkiImage;
    
    Rect srcRect;
    srcRect.left = 0.0f;
    srcRect.top = curImage->getHeight() - srcTop_;
    srcRect.width = curImage->getWidth();
    srcRect.height = 300.0f;

    Rect dstRect;
    dstRect.left = 0.0f;
    dstRect.top = viewHeight_;
    dstRect.width = curImage->getWidth();
    dstRect.height = 300.0f;

    spriteBatch->renderImage(*curImage , srcRect , dstRect);

    Rect originSrcRect;
    originSrcRect.left = 0.0f;
    originSrcRect.top = curImage->getHeight();
    originSrcRect.width = curImage->getWidth();
    originSrcRect.height = curImage->getHeight();

    Rect originDstRect;
    originDstRect.left = dstRect.getRight() + 32.0f;
    originDstRect.top = viewHeight_;
    originDstRect.width = curImage->getWidth();
    originDstRect.height = curImage->getHeight();
    
    spriteBatch->renderImage(*curImage , originSrcRect , originDstRect);

    spriteBatch->end();

    srcTop_ += 1.0f;
    if(srcTop_ + 300.0f >= curImage->getHeight()){
        srcTop_ = 0.0f;
    }
}

void TestDemo::testRenderBlurRect4(){
    Paint bottomPaint;
    bottomPaint.fillStyle = Filled;
    bottomPaint.color = glm::vec4(1.0f , 1.0f , 1.0f ,1.0f);

    Rect bottomRect;
    bottomRect.left = (viewWidth_ - viewHeight_)/2.0f;
    bottomRect.top = viewHeight_;
    bottomRect.height = viewWidth_;
    bottomRect.width = viewHeight_;

    renderEngine_->getShapeBatch()->begin();
    renderEngine_->getShapeBatch()->renderRect(bottomRect , bottomPaint);

    Paint paint;
    paint.color = glm::vec4(0.0f , 0.0f , 0.0f , 1.0f);

    
    Rect rect;
    rect.left = bottomRect.left + bottomRect.width / 4.0;
    rect.height = 100.0f;
    rect.top = (viewHeight_/2.0 + rect.height / 2.0f);
    rect.width = bottomRect.width / 2.0f;

    Rect shadowRect = rect;
    shadowRect.height = 8.0f;
    shadowRect.top -= rect.height;
    
    auto blackColor = glm::vec4(0.0f , 0.0f ,0.0f ,0.5f);
    auto transparentColor = glm::vec4(0.0f , 0.0f ,0.0f ,0.0f);
    renderEngine_->getShapeBatch()
        ->renderLinearGradientRect(shadowRect , blackColor,blackColor , transparentColor,transparentColor);

    paint.color = ConvertColor(100,181,246,255);
    renderEngine_->getShapeBatch()->renderRect(rect , paint);

    renderEngine_->getShapeBatch()->end();
}

void TestDemo::testRenderBlurRect3(){
    Paint bottomPaint;
    bottomPaint.fillStyle = Filled;
    bottomPaint.color = glm::vec4(1.0f , 1.0f , 1.0f ,1.0f);

    Rect bottomRect;
    bottomRect.left = (viewWidth_ - viewHeight_)/2.0f;
    bottomRect.top = viewHeight_;
    bottomRect.height = viewWidth_;
    bottomRect.width = viewHeight_;

    renderEngine_->getShapeBatch()->begin();
    renderEngine_->getShapeBatch()->renderRect(bottomRect , bottomPaint);

    Paint paint;
    paint.color = glm::vec4(0.0f , 0.0f , 0.0f , 1.0f);

    float shadowOffset = 4.0f;
    
    Rect rect;
    rect.left = bottomRect.left + bottomRect.width / 4.0;
    rect.height = 100.0f;
    rect.top = (viewHeight_/2.0 + rect.height / 2.0f);
    rect.width = bottomRect.width / 2.0f;

    Rect shadowRect = rect;
    shadowRect.height = 0.0f;
    shadowRect.left += 7.0f;
    shadowRect.width = rect.width - 16.0f;
    shadowRect.top -= rect.height - 4.5f;
    
    renderEngine_->getShapeBatch()->renderBlurRect(shadowRect , 10.0f , paint);

    paint.color = ConvertColor(100,181,246,255);
    renderEngine_->getShapeBatch()->renderRect(rect , paint);

    renderEngine_->getShapeBatch()->end();
}

void TestDemo::testRenderBlurRect2(){
    Paint paint;
    paint.color = glm::vec4(0.0f , 0.0f , 1.0f , 1.0f);

    float blurSize = 15.0f * glm::sin(deltatime_) + 15.0f;

    deltatime_ += 0.05f;

    renderEngine_->getShapeBatch()->begin();

    float cubeSize = 10.0f;
    float gapSize = 15.0f * 2.0f;

    float x = 0.0f;
    float y = viewHeight_;

    for(;y > 0.0f ; y -= cubeSize + gapSize){
        for(x = 0.0f ; x < viewWidth_; x += cubeSize + gapSize){
            Rect rect;
            rect.left = x;
            rect.top = y;
            rect.width = cubeSize;
            rect.height = cubeSize;

            renderEngine_->getShapeBatch()
                ->renderBlurRect(rect , blurSize , paint);
        }
    }//end for y

   
    renderEngine_->getShapeBatch()->end();
}

void TestDemo::testRenderBlurRect(){
    Paint bottomPaint;
    bottomPaint.fillStyle = Filled;
    bottomPaint.color = glm::vec4(1.0f , 1.0f , 1.0f ,1.0f);

    Rect bottomRect;
    bottomRect.left = (viewWidth_ - viewHeight_)/2.0f;
    bottomRect.top = viewHeight_;
    bottomRect.height = viewWidth_;
    bottomRect.width = viewHeight_;

    renderEngine_->getShapeBatch()->begin();
    renderEngine_->getShapeBatch()->renderRect(bottomRect , bottomPaint);

    Paint paint;
    paint.color = glm::vec4(0.0f , 0.0f , 0.0f , 1.0f);

    Rect rect;
    rect.left = (viewWidth_ - viewHeight_)/2.0f +100.0f;
    rect.top = viewHeight_ / 2.0f;
    rect.width = bottomRect.width / 2.0f;
    rect.height = 0.0f;    

    float blurSize = 30.0f * glm::sin(deltatime_) + 30.0f;

    deltatime_ += 0.05f;

    renderEngine_->getShapeBatch()
        ->renderBlurRect(rect , 40.0f , paint);

    // paint.color = glm::vec4(1.0f  ,0.0f , 0.0f , 1.0f);
    // renderEngine_->getShapeBatch()
    //     ->renderRect(rect , paint);

    renderEngine_->getShapeBatch()->end();
}

void TestDemo::testRenderLinearGradRect(){
    Paint bottomPaint;
    bottomPaint.fillStyle = Filled;
    bottomPaint.color = glm::vec4(1.0f , 1.0f , 1.0f ,1.0f);

    Rect bottomRect;
    bottomRect.left = (viewWidth_ - viewHeight_)/2.0f;
    bottomRect.top = viewHeight_;
    bottomRect.height = viewWidth_;
    bottomRect.width = viewHeight_;

    renderEngine_->getShapeBatch()->begin();
    renderEngine_->getShapeBatch()->renderRect(bottomRect , bottomPaint);

    Paint paint;
    paint.color = glm::vec4(0.0f , 0.0f , 0.0f , 1.0f);

    auto startColor = glm::vec4(0.0f , 0.0f , 0.0f , 1.0f);
    auto endColor = glm::vec4(0.0f , 0.0f , 0.0f , 0.0f);

    Rect rect;
    rect.left = (viewWidth_ - viewHeight_)/2.0f;
    rect.top = viewHeight_ / 2.0f;
    rect.width = 160.0f;
    rect.height = 100.0f;

    renderEngine_->getShapeBatch()
        ->renderLinearGradientRect(rect , 
            glm::vec4(1.0f , 0.0f , 0.0f , 1.0f),
            glm::vec4(0.0f , 1.0f , 0.0f , 1.0f),
            glm::vec4(0.0f , 0.0f , 1.0f , 1.0f),
            glm::vec4(1.0f , 1.0f , 0.0f , 1.0f));

    // renderEngine_->getShapeBatch()->end();
    // renderEngine_->getShapeBatch()->begin();
    Paint paint2;
    paint2.color = glm::vec4(0.0f , 0.0f , 0.0f , 1.0f);

    float radius = 100.0f;
    float centerX = viewWidth_ / 2.0f;
    float centerY = viewHeight_ / 2.0f;

    float shadowOffset = 4.0f;
    renderEngine_->getShapeBatch()
        ->renderBlurCircle(centerX + shadowOffset, centerY - 2*shadowOffset, 
            0.4f * radius , radius * 0.8f , paint2);
    
    paint2.color = ConvertColor(100,181,246,255);
    renderEngine_->getShapeBatch()
        ->renderCircle(centerX , centerY , radius , paint2);

    renderEngine_->getShapeBatch()->end();
}

void TestDemo::testRenderBlurCircle(){
    Paint bottomPaint;
    bottomPaint.fillStyle = Filled;
    bottomPaint.color = glm::vec4(1.0f , 1.0f , 1.0f ,1.0f);

    Rect bottomRect;
    bottomRect.left = (viewWidth_ - viewHeight_)/2.0f;
    bottomRect.top = viewHeight_;
    bottomRect.height = viewWidth_;
    bottomRect.width = viewHeight_;

    renderEngine_->getShapeBatch()->begin();
    renderEngine_->getShapeBatch()->renderRect(bottomRect , bottomPaint);

    Paint paint;
    paint.color = glm::vec4(0.0f , 0.0f , 0.0f , 1.0f);

    float radius = 100.0f;
    float centerX = viewWidth_ / 2.0f;
    float centerY = viewHeight_ / 2.0f;

    float shadowOffset = 4.0f;
    renderEngine_->getShapeBatch()
        ->renderBlurCircle(centerX + shadowOffset, centerY - shadowOffset, 
            0.4f * radius , radius * 0.8f , paint);
    
    paint.color = ConvertColor(100,181,246,255);
    renderEngine_->getShapeBatch()
        ->renderCircle(centerX , centerY , radius , paint);

    renderEngine_->getShapeBatch()->end();
}


void TestDemo::testRenderTableTennis(){
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

void TestDemo::testRenderRoundRect(){
    Paint paint;
    paint.fillStyle = Filled;
    paint.stokenWidth = 1.0f;
    paint.color = glm::vec4(0.0f , 0.0f , 0.0f , 1.0f);

    float width = 400.0f;
    float height = 400.0f;
    float left = viewWidth_ / 2.0f - width / 2.0f;
    float top = viewHeight_ / 2.0f + height / 2.0f;
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

void TestDemo::testRender4(){
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
    for(; y <= viewHeight_+ height; y += height + padding){
        for(x = 0.0f;x <= viewWidth_ ; x += width + padding){
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

void TestDemo::testRender3(){
    Paint circelPaint;
    circelPaint.fillStyle = Stroken;
    circelPaint.stokenWidth = 1.0f;
    circelPaint.color = glm::vec4(0.0f , 1.0f , 0.0f , 1.0f);

    mRadius++;

    float cx = viewWidth_ / 2.0f;
    float cy = viewHeight_ / 2.0f;
    renderEngine_->getShapeBatch()->begin();

    float width = 32.0f;
    float height = 32.0f;
    float x = 0.0f;
    float y = height;
    float padding = 2.0f;
    int rectCount = 0;
    long long t1 = currentTimeMicro();
    for(; y <= viewHeight_ ; y += height + padding){
        for(x = 0.0f;x <= viewWidth_ ; x += width + padding){
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

    if(mRadius > viewHeight_ / 2.0f){
        mRadius = 0.0f;
    }
}

void TestDemo::testRender2(){
    Paint circelPaint;
    circelPaint.fillStyle = Filled;
    circelPaint.stokenWidth = 1.0f;
    circelPaint.color = glm::vec4(0.0f , 1.0f , 0.0f , 1.0f);

    float radius = 16.0f;
    float cx = radius;
    float cy = viewHeight_ - radius;
    float padding = 4.0f;

    bool isFilled = false;
    renderEngine_->getShapeBatch()->begin();
    for(;cy >= 0; cy -= 2*radius + padding){
        cx = radius;
        for(;cx <= viewWidth_ ; cx += 2*radius + padding){
            circelPaint.fillStyle = GenRandomFloat()>0.5f?Filled:Stroken;
            // renderEngine_->renderCircle(cx , cy , radius , circelPaint);
            circelPaint.color = glm::vec4(GenRandomFloat() , GenRandomFloat() , GenRandomFloat() , 1.0f);
            renderEngine_->getShapeBatch()->renderCircle(cx , cy , radius , circelPaint);
            isFilled = !isFilled;
        }
    }//end for y
    renderEngine_->getShapeBatch()->end();
}

void TestDemo::testRender1(){
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
    for(; y <= viewHeight_ ; y += height + padding){
        for(x = 0.0f;x <= viewWidth_ ; x += width + padding){
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

void TestDemo::testAudio(){
    // AudioManager::getInstance()->loadMusic("musicA" , "music_name");
    // AudioManager::getInstance()->loadSound("soundA" , "soundA_name");
    // AudioManager::getInstance()->playSound("soundA");
    // AudioManager::getInstance()->playMusic("musicA");
    // AudioManager::getInstance()->dispose();
}

void TestDemo::testRenderShader(){
    Rect viewRect;
    viewRect.left = 0.0f;
    viewRect.top = viewHeight_;
    viewRect.width = viewWidth_;
    viewRect.height = viewHeight_;

//    Paint paint;
//    paint.color = glm::vec4 (1.0f , 0.0f , 0.0f ,0.3f);
//    renderEngine_->getShapeBatch()->begin();
//    renderEngine_->getShapeBatch()->renderRect(viewRect , paint);
//    renderEngine_->getShapeBatch()->end();

    // Logi("ShaderDemo" , "testRenderShader");
    renderEngine_->renderShader(customRenderShader , viewRect, [this](){
        customRenderShader.setUniformFloat("uViewWidth",viewWidth_);
        customRenderShader.setUniformFloat("uViewHeight",viewHeight_);
    });
}
