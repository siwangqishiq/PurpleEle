#pragma once

#include "application.hpp"

class RenderEngine;
class TextureImage;

class TestDemo :public EventActionCallback{
public:
    TestDemo(Application *app){
        appContext = app;
    }

    void init();

    void tick();

    void dispose();

    virtual bool onEventAction(int action , float x , float y);

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

    std::shared_ptr<TextureImage> yuanImage_;

    Shader customRenderShader;

    float srcTop_ = 0.0f;

    float rotateAngle = 0.0f;

    int walkIndex = 0;

    std::wstring code_;

    int count_ = 0;

    float deltatime_ = 0.0f;

    void testAudio();
    
    void testRender1();
    void testRender2();
    void testRender3();
    void testRender4();
    void testRenderRoundRect();
    void testRenderTableTennis();

    void testRenderBlurCircle();
    void testRenderLinearGradRect();
    void testRenderBlurRect();
    void testRenderBlurRect2();
    void testRenderBlurRect3();
    void testRenderBlurRect4();

    void testRenderSprite1();
    void testRenderSprite2();
    void testRenderSprite3();
    void testRenderSprite4();
    void testRenderSprite5Rotate();
    void testRenderSprite6();

    void testRenderShader();

    void testRenderSprite6ImageRegion();

    void testRenderText();
    void testRenderTextWithRect();
    void testRenderTextGravity();
    void testRenderTextCode();

    void testActionDown();
};