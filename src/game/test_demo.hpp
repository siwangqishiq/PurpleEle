#pragma once

#include "application.hpp"

class RenderEngine;

class TestDemo{
public:
    TestDemo(Application *app){
        appContext = app;
    }

    void init();

    void tick();

    void dispose();

private:
    Application *appContext;

    int viewWidth_;
    int viewHeight_;

    std::shared_ptr<RenderEngine> renderEngine_;

    float mRadius = 1.0f;
    
    void testRender1();
    void testRender2();
    void testRender3();
    void testRender4();
    void testRenderRoundRect();
    void testRenderTableTennis();
};