#include "test_demo.hpp"
#include "utils.hpp"
#include "render/common.hpp"
#include "render/render.hpp"
#include "render/render_batch.hpp"
#include "audio/audio.hpp"
#include "render/sprite.hpp"
#include <cmath>
#include "widget/timer.hpp"

void TestDemo::init(){
    viewWidth_ = appContext->viewWidth_;
    viewHeight_ = appContext->viewHeight_;

    renderEngine_ = appContext->getRender();

    testAudio();
    testImage = BuildImageByAsset("lan.jpg");
    testGakkiImage = BuildImageByAsset("test.jpeg");
    wallpaperImage = BuildImageByAsset("bizhi.jpg");
    walkingImage = BuildImageByAsset("sprite/walk.png");//32 x 48
    
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

    GLint size = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE , &size);
    Logi("testDemo" , "max texture size = %d"  ,size);
}

void TestDemo::tick(){
    // testRender1();
    // testRender2();
    // testRender3();
    // testRender4();
    // testRenderRoundRect();
    // testRenderTableTennis();
    // testRenderSprite1();
    // testRenderSprite2();
    // testRenderSprite3();
    // testRenderSprite4();

    testRenderText();
    testRenderSprite5Rotate();
    // testRenderSprite6ImageRegion();
    // testRenderShader();
   
}

void TestDemo::dispose(){
    
}

void TestDemo::testRenderText(){
    TextPaint paint;
    paint.setTextSize(32.0f);
    renderEngine_->renderText(L"Hello World! Thank    you quit?" , 
        100.0, 200.0 , paint);
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
    
    spriteBatch->renderImage(*curImage , originSrcRect , originDstRect
);

    spriteBatch->end();

    srcTop_ += 1.0f;
    if(srcTop_ + 300.0f >= curImage->getHeight()){
        srcTop_ = 0.0f;
    }
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
    AudioManager::getInstance()->loadMusic("musicA" , "music_name");
    AudioManager::getInstance()->loadSound("soundA" , "soundA_name");

    AudioManager::getInstance()->playSound("soundA");
    AudioManager::getInstance()->playMusic("musicA");
    
    AudioManager::getInstance()->dispose();
}

void TestDemo::testRenderShader(){

}
