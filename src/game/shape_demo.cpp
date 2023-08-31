#include "shape_demo.hpp"
#include "../render/render.hpp"
#include "../render/render_batch.hpp"
#include <memory>
#include "glm/gtc/quaternion.hpp"

void ShapeDemo::init(){
    viewWidth_ = appContext->viewWidth_;
    viewHeight_ = appContext->viewHeight_;

    renderEngine_ = appContext->getRender();
    appContext->addEventActionCallback(this);
    appContext->isShowFps = true;

    //todo init
    
    if(isInited){
        return;
    }
    isInited = true;
}

void ShapeDemo::buildViews(){
}

void ShapeDemo::tick(){
    Rect rect;

    rect.width = viewHeight_ / 4.0f;
    rect.height = viewHeight_ / 2.0f;
    rect.left = 0.0f;
    rect.top = rect.height;

    Paint paint;
    paint.color = COLOR_SKY_BLUE;
    
    static float angle = 0.0f;
    glm::mat4 mat(1.0f);
    auto center = rect.center();
    // mat = glm::translate(mat , glm::vec3(center.x , center.y , 0.0f));
    mat = glm::rotate(mat , glm::radians(angle) , glm::vec3(0.0, 0.0 , 1.0f));
    // mat = glm::scale(mat , glm::vec3(1.0f , 1.0f , 1.0f));
    // mat = glm::translate(mat , glm::vec3(-center.x , -center.y , 0.0f));
    // mat = glm::translate(mat , glm::vec3(rect.left , 0.0f , 0.0f));

    renderEngine_->renderRect(rect , mat , paint);
    angle++;
}

void ShapeDemo::dispose(){
    appContext->removeEventActionCallback(this);
}

bool ShapeDemo::onEventAction(int action , float x , float y){
    return false;
}



