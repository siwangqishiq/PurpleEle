#include "test_demo.hpp"
#include "utils.hpp"
#include "render/common.hpp"
#include "render/render.hpp"
#include "render/render_batch.hpp"
#include "audio/audio.hpp"
#include "render/sprite.hpp"

void TestDemo::init(){
    viewWidth_ = appContext->viewWidth_;
    viewHeight_ = appContext->viewHeight_;

    renderEngine_ = appContext->getRender();

    testAudio();
    testImage = BuildImageByAsset("lan.jpg");
    // testImage = BuildImageByAsset("test.jpeg");

    Logi("testDemo" , "testImage w : %d , h : %d" , 
        testImage->getWidth(),testImage->getHeight());
}

void TestDemo::tick(){
    // testRender1();
    // testRender2();
    // testRender3();
    // testRender4();
    // testRenderRoundRect();
    testRenderTableTennis();
    testRenderSprite1();
}

void TestDemo::dispose(){

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
