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

    // int lineWidth = 0;
    // glGetIntegerv(GL_LINE_WIDTH , &lineWidth);
    // std::cout << "line Width = " << lineWidth << std::endl;
    
    // GLfloat min_width, max_width;
    // glGetFloatv(GL_LINE_WIDTH_RANGE, &min_width);
    // glGetFloatv(GL_LINE_WIDTH_RANGE + 1, &max_width);
    // std::cout << "min_width = " << min_width << " max_width = " << max_width << std::endl;

    // float lineWidth[2];
    // glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidth);
    // std::cout << "line min Width = " << lineWidth[0] <<
    //     "  line max Width = " << lineWidth[1] << std::endl;
}

void ShapeDemo::buildViews(){
}

void ShapeDemo::tick(){
    // testRenderShapeRect();
    // testRenderLines();
    // testRenderLines2();
    testRenderLinesStar();
}

void ShapeDemo::testRenderLinesStar(){
    glm::vec2 center(viewWidth_ / 2.0f , viewHeight_ / 2.0f);

    const uint8_t iter = 5;
    glm::vec2 p[iter];
    float deltaAngleRad = 2*PI / iter;
    float radius = viewHeight_ / 3.0f;
    float angle = deltaAngleRad / 4.0f;
    for(int i = 0 ; i< iter ;i++){
        p[i][0] = center[0] + radius * glm::cos(angle);
        p[i][1] = center[1] + radius * glm::sin(angle);

        angle += deltaAngleRad;
    }//end for i

    std::vector<float> points(2*(iter + 1));
    points[0] = p[0][0];
    points[1] = p[0][1];

    points[2] = p[2][0];
    points[3] = p[2][1];

    points[4] = p[4][0];
    points[5] = p[4][1];

    points[6] = p[1][0];
    points[7] = p[1][1];

    points[8] = p[3][0];
    points[9] = p[3][1];

    points[10] = p[0][0];
    points[11] = p[0][1];

    Paint paint;
    paint.stokenWidth = 16.0f;
    paint.color = glm::vec4(1.0f , 0.0f , 0.0f , 1.0f);
    renderEngine_->renderLines(points , paint);
}

void ShapeDemo::testRenderLines2(){
    Paint paint;
    paint.stokenWidth = 20.0f;
    paint.color = glm::vec4(1.0f , 1.0f , 0.0f , 1.0f);

    std::vector<float> points(2 * 3);
    points[0] = 0.0f;
    points[1] = 0.0f;

    points[2] = viewWidth_ / 2.0f;
    points[3] = viewHeight_ / 2.0f;

    points[4] = viewWidth_;
    points[5] = 0.0f;

    renderEngine_->renderLines(points , paint);
}

void ShapeDemo::testRenderLines(){
    Paint paint;
    paint.stokenWidth = 8.0f;
    paint.color = glm::vec4(1.0f , 1.0f , 1.0f , 1.0f);

    std::vector<float> points(2 * 3);
    points[0] = 30.0f;
    points[1] = 30.0f;

    points[2] = viewWidth_ / 2.0f;
    points[3] = viewHeight_ / 2.0f;

    points[4] = viewWidth_;
    points[5] = 0.0f;

    renderEngine_->renderLines(points , paint);
}

void ShapeDemo::testRenderShapeRect(){
    Rect rect;

    rect.width = viewHeight_ / 4.0f;
    rect.height = viewHeight_ / 2.0f;
    rect.left = 0.0f;
    rect.top = rect.height;

    Paint paint;
    paint.color = COLOR_SKY_BLUE;
    paint.fillStyle = Stroken;
    
    static float angle = 0.0f;
    glm::mat4 mat(1.0f);
    auto center = rect.center();
    mat = glm::translate(mat , glm::vec3(center.x , center.y , 0.0f));
    mat = glm::rotate(mat , glm::radians(angle) , glm::vec3(0.0, 0.0 , 1.0f));
    // mat = glm::scale(mat , glm::vec3(1.0f , 1.0f , 1.0f));
    mat = glm::translate(mat , glm::vec3(-center.x , -center.y , 0.0f));
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



