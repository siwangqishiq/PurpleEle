#include "shape_demo.hpp"
#include "../render/render.hpp"
#include "../render/render_batch.hpp"
#include <memory>
#include "glm/gtc/quaternion.hpp"
#include "render/sprite.hpp"

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

    image_ = BuildImageByAsset("bg.jpg");
    customTextureShader_ = 
        ShaderManager::getInstance()->loadAssetShader("custom_texture_shader2",
                    "shader/sprite_batch_vertex.glsl", 
                    "shader/custom_texture_shader.glsl");

    emptyTex_ = BuildEmptyImage(512 , 512 , GL_RED);
}

void ShapeDemo::buildViews(){
}

void ShapeDemo::tick(){
    // testRenderShapeRect();
    // testRenderLines();
    // testRenderLines2();
    // testRenderLinesStar();
    // testRenderArc();
    // testRenderArc2();
    // testRenderArc3();
    // testCustomTextureShader();
    // testRenderShapeRect2();
    testEmptyTexture();
}

void ShapeDemo::testEmptyTexture(){
    Rect rect;
    rect.left = 0.0f;
    rect.top = viewHeight_;
    rect.width = viewHeight_;
    rect.height = viewHeight_;

    const int texSize = emptyTex_->getWidth() * emptyTex_->getHeight();
    uint8_t *texData = new uint8_t[texSize];
    for(int j = 0; j < emptyTex_->getHeight() ; j++){
        for(int i = 0 ; i < emptyTex_->getWidth() ; i++){
            if(i == j){
                texData[j * emptyTex_->getWidth() + i] = 100;
            }else{
                texData[j * emptyTex_->getWidth() + i] = 255;
            }
        }
    }

    emptyTex_->updateTextureData(texData);
    delete[] texData;

    renderEngine_->renderTextureShader(customTextureShader_ , 
        rect , 0, 
        [this](){
            customTextureShader_.setUniformInt("mainTexture", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D , emptyTex_->getTextureId());
        });
}

void ShapeDemo::testCustomTextureShader(){
    Rect rect;
    rect.left = 0.0f;
    rect.top = viewHeight_;
    rect.width = viewHeight_;
    rect.height = viewHeight_;

    renderEngine_->renderTextureShader(customTextureShader_ , rect , 
        image_->getTextureId() , nullptr);
    TextPaint paint;
    paint.textColor = COLOR_SKY_BLUE;
    paint.setTextSize(64.0f);
    renderEngine_->renderText(L"你好世界", rect.width ,rect.top - 200.0f , paint);
}



void ShapeDemo::testRenderArc3() {
    static float time = 0.0f;
    float stokenWidth = 50.0f * glm::sin(time) + 50.0f;
    time += 0.01f;

    Paint paint;
    paint.color = COLOR_SKY_BLUE;
    paint.fillStyle = Stroken; 
    paint.stokenWidth = stokenWidth;

    // arc1
    float radius = viewHeight_ / 4.0f;
    float centerX = viewWidth_  / 2.0f;
    float centerY = viewHeight_ / 2.0f;

    glm::vec2 leftArcCenter = glm::vec2(centerX - radius - radius , centerY);
    glm::vec2 rightArcCenter = glm::vec2(centerX + radius + radius , centerY);

    renderEngine_->renderArc(leftArcCenter[0] , leftArcCenter[1] 
        , radius , 90.0f , 270.0f ,false, paint);

    std::vector<float> line1(2*2);
    line1[0] = leftArcCenter[0] + paint.stokenWidth / 2.0f;
    line1[1] = leftArcCenter[1] + radius;

    line1[2] = rightArcCenter[0] - paint.stokenWidth / 2.0f;
    line1[3] = rightArcCenter[1] + radius;
    renderEngine_->renderLines(line1 , paint);

    renderEngine_->renderArc(rightArcCenter[0] , rightArcCenter[1] 
        , radius , 90.0f , 270.0f ,true, paint);

    std::vector<float> line2(2*2);
    line2[0] = leftArcCenter[0] + paint.stokenWidth / 2.0f;
    line2[1] = leftArcCenter[1] - radius;

    line2[2] = rightArcCenter[0]- paint.stokenWidth / 2.0f;
    line2[3] = rightArcCenter[1] - radius;
    renderEngine_->renderLines(line2 , paint);
}

void ShapeDemo::testRenderArc2() {
    Paint paint;
    paint.color = glm::vec4(1.0f , 0.0f , 0.0f , 1.0f);
    paint.fillStyle = Stroken; 
    paint.stokenWidth = 20.0f;

    float cx = 200.0f;
    float cy = viewHeight_ / 2.0f;

    float radius = 200.0f;
    static float angle = 0.0f;
    angle += 1.0f;
    renderEngine_->renderArc(cx , cy , radius , angle , 360.0f ,true, paint);
    renderEngine_->renderArc(cx + radius + radius , cy , radius , angle , 360.0f ,false, paint);
}

void ShapeDemo::testRenderArc(){
    Paint paint;
    paint.color = COLOR_SKY_BLUE;

    float cx = 200.0f;
    float cy = viewHeight_ / 2.0f;

    float radius = 200.0f;
    static float angle = 0.0f;
    angle += 1.0f;
    renderEngine_->renderArc(cx , cy , radius , angle , 270.0f , true, paint);
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
    paint.fillStyle = Filled;
    
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

void ShapeDemo::testRenderShapeRect2(){
    Rect rect;

    rect.width = viewHeight_ / 4.0f;
    rect.height = viewHeight_ / 2.0f;
    rect.left = viewWidth_ / 2.0f - rect.width / 2.0f;
    rect.top = viewHeight_/ 2.0f + rect.height/2.0f;

    Paint paint;
    paint.color = COLOR_SKY_BLUE;
    paint.fillStyle = Stroken;
    paint.stokenWidth = 20.0f;
    
    static float angle = 0.0f;
    glm::mat4 mat(1.0f);
    auto center = rect.center();
    mat = glm::translate(mat , glm::vec3(center.x , center.y , 0.0f));
    mat = glm::rotate(mat , glm::radians(angle) , glm::vec3(0.0, 0.0 , 1.0f));
    mat = glm::translate(mat , glm::vec3(-center.x , -center.y , 0.0f));
    
    renderEngine_->renderRect(rect , mat , paint);
    angle++;
}

void ShapeDemo::dispose(){
    appContext->removeEventActionCallback(this);
}

bool ShapeDemo::onEventAction(int action , float x , float y){
    return false;
}



