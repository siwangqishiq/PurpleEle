#include "custom_shader_demo.hpp"
#include "../render/render_batch.hpp"
#include "../render/render.hpp"
#include <memory>
#include "glm/vec4.hpp"

void CustomShaderDemo::init(){
    viewWidth_ = appContext->viewWidth_;
    viewHeight_ = appContext->viewHeight_;

    renderEngine_ = appContext->getRender();
    appContext->addEventActionCallback(this);
    appContext->isShowFps = true;
        
    if(isInited){
        return;
    }
    isInited = true;
    

    //shader render a heart
    customRenderShader_ = ShaderManager::getInstance()->loadAssetShader(
        "customshader1","shader/shader_vert.glsl","shader/customshader_heart_frag.glsl");
    
    //ray trace step 
    // customRenderShader_ = ShaderManager::getInstance()->loadAssetShader(
    //     "raytrace","shader/shader_vert.glsl","shader/customshader_raytrace3_frag.glsl");
}

void CustomShaderDemo::buildViews(){
   
}

void CustomShaderDemo::tick(){
    Rect rect;
    float size = viewHeight_ / 2.0f;
    rect.width = size;
    rect.height = size;
    rect.top = viewHeight_ / 2.0f + size / 2.0f;
    rect.left = viewWidth_ / 2.0f - size / 2.0f;
    renderEngine_->renderShader(customRenderShader_ , rect, [this , rect](){
        customRenderShader_.setUniformFloat("uLeft",rect.left);
        customRenderShader_.setUniformFloat("uTop",rect.top);
        customRenderShader_.setUniformFloat("uWidth",rect.width);
        customRenderShader_.setUniformFloat("uHeight",rect.height);
        customRenderShader_.setUniformVec4("uColor",glm::vec4(1.0f , 0.0f ,0.0f , 1.0f));
    });
    

    //ray trace
    // Rect screenRect;
    // screenRect.left = 0.0f;
    // screenRect.top = viewHeight_;
    // screenRect.width = viewWidth_;
    // screenRect.height = viewHeight_;
    // renderEngine_->renderShader(customRenderShader_ , screenRect, [this , screenRect](){
    //     customRenderShader_.setUniformFloat("uViewWidth",screenRect.width);
    //     customRenderShader_.setUniformFloat("uViewHeight",screenRect.height);
    //     customRenderShader_.setUniformFloat("uTime" , static_cast<float>(appContext->frameCount_));
    //     customRenderShader_.setUniformFloat("uFuzz" , 
    //         static_cast<float>(0.5f * glm::sin(delta_) + 0.5f));
    //     customRenderShader_.setUniformFloat("uDeltaY" , 0.3f * glm::sin(delta_));
    // });
    // delta_ += 0.04f;
}

void CustomShaderDemo::dispose(){

}

bool CustomShaderDemo::onEventAction(int action , float x , float y){
    return false;
}



