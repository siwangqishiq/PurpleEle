#pragma once

#include "application.hpp"

class RenderEngine;
class TextureImage;

class TestDemo{
public:
    TestDemo(Application *app){
        appContext = app;
    }

    void init();

    void tick();

    void dispose();

    bool isInited = false;
private:
    Application *appContext;

    int viewWidth_;
    int viewHeight_;

    std::shared_ptr<RenderEngine> renderEngine_;

    float mRadius = 1.0f;

    std::shared_ptr<TextureImage> testImage;
    std::shared_ptr<TextureImage> testGakkiImage;
    std::shared_ptr<TextureImage> wallpaperImage;
    std::shared_ptr<TextureImage> walkingImage;

    Shader customRenderShader;

    float srcTop_ = 0.0f;

    float rotateAngle = 0.0f;

    int walkIndex = 0;

    void testAudio();
    
    void testRender1();
    void testRender2();
    void testRender3();
    void testRender4();
    void testRenderRoundRect();
    void testRenderTableTennis();

    void testRenderSprite1();
    void testRenderSprite2();
    void testRenderSprite3();
    void testRenderSprite4();
    void testRenderSprite5Rotate();

    void testRenderShader();

    void testRenderSprite6ImageRegion();

    void testRenderText();
    void testRenderTextWithRect();
    void testRenderTextGravity();
};