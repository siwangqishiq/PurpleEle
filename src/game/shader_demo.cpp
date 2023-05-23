#include "shader_demo.hpp"
#include "utils.hpp"
#include "render/common.hpp"
#include "render/render.hpp"
#include "render/render_batch.hpp"
#include "render/sprite.hpp"

void ShaderDemo::init(){
    viewWidth_ = appContext->viewWidth_;
    viewHeight_ = appContext->viewHeight_;
    renderEngine_ = appContext->getRender();
    
    if(isInited){
        return;
    }
    isInited = true;

    customRenderShader = ShaderManager::getInstance()->loadAssetShader(
        "shader_demo","shader/shader_vert.glsl","shader/shader_frag.glsl");
}

void ShaderDemo::tick(){
    testRenderShader();
}

void ShaderDemo::dispose(){
    
}


void ShaderDemo::testRenderShader(){
    Rect viewRect;
    viewRect.left = 0.0f;
    viewRect.top = viewHeight_;
    viewRect.width = viewWidth_;
    viewRect.height = viewHeight_;

    Paint paint;
    renderEngine_->getShapeBatch()->begin();
    renderEngine_->getShapeBatch()->renderRect(viewRect , paint);
    renderEngine_->getShapeBatch()->end();

    // Logi("ShaderDemo" , "testRenderShader");
    renderEngine_->renderShader(customRenderShader , viewRect, [this](){
        customRenderShader.setUniformFloat("uViewWidth",viewWidth_);
        customRenderShader.setUniformFloat("uViewHeight",viewHeight_);
    });
}
