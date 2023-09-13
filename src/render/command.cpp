#include "command.hpp"
#include "render.hpp"
#include "log.hpp"
#include "vram.hpp"
#include "../../libjson/json.hpp"
#include "common.hpp"
#include "application.hpp"

TextPaint TextRenderCommand::defaultTextPaint;

unsigned int RenderCommand::allocatorVRam(int acquireSize , int &allocateSize){
    engine_->vramManager_->fetchVideoMemory(acquireSize , vbo_ ,vao_, vboOffset_ , allocateSize);
    return vbo_;
}

void RenderCommand::buildGlCommands(std::vector<float> &buf){
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER , vbo_);
    glBufferSubData(GL_ARRAY_BUFFER , vboOffset_ ,buf.size() * sizeof(float) , buf.data());
    glBindBuffer(GL_ARRAY_BUFFER , 0);
    glBindVertexArray(0);
}


float TextRenderCommand::findCharMaxHeight(std::wstring &text , TextPaint &paint){
    auto textRenderHelper = engine_->textRenderHelper_;
    float maxHeight = 0.0f;
    for(int i = 0 ; i < text.length() ;i++){
        wchar_t ch = text[i];
        auto charInfoPtr = textRenderHelper->findCharInfo(ch);
        if(charInfoPtr == nullptr){
            continue;
        }
        
        float realHeight = charInfoPtr->height * defaultTextPaint.textSizeScale;
        if(maxHeight < realHeight) {
            maxHeight = realHeight;
        }
    }//end for i
    return maxHeight;
}

float TextRenderCommand::calOffsetY(std::shared_ptr<CharInfo> charInfo , float scale){
    if(TextRenderHelper::isSymbol(charInfo->value)){
        return 0.0f;
    }else{
        float charRealHeight = charInfo->height * scale;
        float fontMaxHeight = FONT_DEFAULT_SIZE * scale;
        return fontMaxHeight / 2.0f - charRealHeight / 2.0f;
    }
}

float TextRenderCommand::calTextStyleItalicOffset(std::shared_ptr<CharInfo> charInfo , TextPaint &paint){
    // Logi("italic" , "offset %d" , paint.textStyle);
    if(charInfo == nullptr || paint.textStyle == TextStyle::normal){
        return 0.0f;
    }else if(paint.textStyle == TextStyle::italic){
        return (FONT_DEFAULT_SIZE * paint.textSizeScale) / 3.0f;
    }
    return 0.0f;
}

unsigned int TextRenderCommand::allocatorVRamForText(int textLength){
    vertexCount_ = vertCountPerChar_ * textLength;
    // Logi("TextRenderCommand" , "vertexcount = %d", vertexCount_);
    int requestSize = vertexCount_ * attrCount_ * sizeof(float);
    int allocateSize = 0;
    // Logi("command" , "allocator size = %d" , requestSize);
    allocatorVRam(requestSize , allocateSize);
    return vbo_;
}

void TextRenderCommand::putParams(std::wstring text 
        ,float left , float bottom
        ,TextPaint &paint){
    paint_ = paint;
    fontTextureId_ = engine_->textRenderHelper_->mainTextureId_;

    // auto t1 = currentTimeMillis();
    allocatorVRamForText(text.length());
    // Logi("TextRenderCommand" , "allocatorVRamForText costtime : %lld" , (currentTimeMillis() - t1));

    if(vbo_ <= 0){
        return;
    }
    // Logi("command" , "allocator vbo = %d" , vbo_);

    float x = left;
    float y = bottom;

    long long t2 = currentTimeMicro();
    auto textRenderHelper = engine_->textRenderHelper_;
    std::vector<float> buf(vertexCount_ * attrCount_);

    float depth = engine_->getAndChangeDepthValue();
    for(int i = 0 ; i < text.length() ;i++){
        wchar_t ch = text[i];
        auto charInfoPtr = textRenderHelper->findCharInfo(ch);
        // Logi("TextRenderCommand" , "%f %f" 
        //     , charInfoPtr->textureCoords[2],charInfoPtr->textureCoords[3]);
        putVertexDataToBuf(buf , i , x , y ,depth, charInfoPtr , paint);
        x += (charInfoPtr->width + paint.gapSize) * paint.textSizeScale;
    }//end for i
    // Logi("TextRenderCommand" , "layout vertex vertexcount : %d , costtime =  %lld" 
    //     , vertexCount_
    //     , (currentTimeMicro() - t2));

    // auto t3 = currentTimeMillis();
    buildGlCommands(buf);
    // Logi("TextRenderCommand" , "buildGlCommands time : %lld" , (currentTimeMillis() - t3));
}

void TextRenderCommand::putTextParamsByRectLimit(std::wstring &text , 
        Rect &limitRect, 
            Rect *wrapContentRect,
            TextPaint &paint){
    if(text.empty()){
        return;
    }

    allocatorVRamForText(text.length());

    paint_ = paint;
    fontTextureId_ = engine_->textRenderHelper_->mainTextureId_;

    // Logi("debug" , "vertexCount = %d , attrCound = %d" , vertexCount_ , attrCount_);
    std::vector<float> buf(vertexCount_ * attrCount_);
    Rect outRect;
    engine_->textRenderHelper_->layoutText(text , this, outRect, buf);
    if(wrapContentRect != nullptr){
        *wrapContentRect = outRect;
    }
    buildGlCommands(buf);
}

void TextRenderCommand::updateVertexPositionData(std::vector<float> &buf, 
        int index, float translateX,float translateY){
    const int attrPerChar = attrCount_ * vertCountPerChar_;
    int offset = index * attrPerChar;

     //v1
    buf[offset + 0] += translateX;
    buf[offset + 1] += translateY;

    //v2
    buf[offset + 6] += translateX;
    buf[offset + 7] += translateY;

    //v3
    buf[offset + 12] += translateX;
    buf[offset + 13] += translateY;
    
    //v4
    buf[offset + 18] += translateX;
    buf[offset + 19] += translateY;

    //v5
    buf[offset + 24] += translateX;
    buf[offset + 25] += translateY;

    //v6
    buf[offset + 30] += translateX;
    buf[offset + 31] += translateY;
}

void TextRenderCommand::putVertexDataToBuf(std::vector<float> &buf, 
        int index,float x ,float y,float depthValue,
        std::shared_ptr<CharInfo> charInfoPtr ,
        TextPaint &paint){
    const int attrPerChar = attrCount_ * vertCountPerChar_;
    const float sizeScale = paint_.textSizeScale;
    // Logi("text_render" , "size scale %f" , sizeScale);
    // Logi("text_render" , "x y %f %f" , x , y);

    float charRealWidth = charInfoPtr->width * sizeScale;
    float charRealHeight = charInfoPtr->height * sizeScale;

    float italicOffset = calTextStyleItalicOffset(charInfoPtr , paint_);
    // Logi("text_render" , "x y %f %f charRealWidth %f , charRealHeight %f" ,
    //     x, y ,charRealWidth,charRealHeight);

    float texLeft = charInfoPtr->textureCoords[0];
    float texTop = charInfoPtr->textureCoords[1];
    float texRight = charInfoPtr->textureCoords[2];
    float texBottom = charInfoPtr->textureCoords[3];
    float texW = charInfoPtr->textureCoords[4];

    //eg: 一 need a offset in y ax
    float offsetX = 0.0f;
    float offsetY = (charInfoPtr->bearingY - charInfoPtr->height) * sizeScale;

    int offset = index * attrPerChar;

    //v1
    buf[offset + 0] = x + offsetX;
    buf[offset + 1] = y + offsetY;
    buf[offset + 2] = depthValue;
    buf[offset + 3] = texLeft;
    buf[offset + 4] = texBottom;
    buf[offset + 5] = texW;
    
    //v2
    buf[offset + 6] = x + charRealWidth + offsetX;
    buf[offset + 7] = y + offsetY;
    buf[offset + 8] = depthValue;
    buf[offset + 9] = texRight;
    buf[offset + 10] = texBottom;
    buf[offset + 11] = texW;

    //v3
    buf[offset + 12] = x + charRealWidth + offsetX + italicOffset;
    buf[offset + 13] = y + offsetY + charRealHeight;
    buf[offset + 14] = depthValue;
    buf[offset + 15] = texRight;
    buf[offset + 16] = texTop;
    buf[offset + 17] = texW;
    
    //v4
    buf[offset + 18] = x + offsetX;
    buf[offset + 19] = y + offsetY;
    buf[offset + 20] = depthValue;
    buf[offset + 21] = texLeft;
    buf[offset + 22] = texBottom;
    buf[offset + 23] = texW;

    //v5
    buf[offset + 24] = x + charRealWidth + offsetX + italicOffset;
    buf[offset + 25] = y + offsetY + charRealHeight;
    buf[offset + 26] = depthValue;
    buf[offset + 27] = texRight;
    buf[offset + 28] = texTop;
    buf[offset + 29] = texW;

    //v6
    buf[offset + 30] = x + offsetX + italicOffset;
    buf[offset + 31] = y + offsetY + charRealHeight;
    buf[offset + 32] = depthValue;
    buf[offset + 33] = texLeft;
    buf[offset + 34] = texTop;
    buf[offset + 35] = texW;
}


void TextRenderCommand::buildGlCommands(std::vector<float> &buf){
    glBindVertexArray(vao_);
    // Logi("cmd" , "vboOffset_ = %d",vboOffset_);
    glBindBuffer(GL_ARRAY_BUFFER , vbo_);
    glBufferSubData(GL_ARRAY_BUFFER , vboOffset_ , 
        buf.size() * sizeof(float) , buf.data());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 6 * sizeof(float) , 
        reinterpret_cast<void *>(vboOffset_));
    glVertexAttribPointer(1 , 3 , GL_FLOAT , GL_FALSE , 6 * sizeof(float) , 
        reinterpret_cast<void *>(vboOffset_ + 3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER , 0);
    glBindVertexArray(0);
}

void TextRenderCommand::runCommands(){
    if(fontTextureId_ <= 0){
        return;
    }
    // Logi("TextRenderCommand" , "runCommands");

    //打开混合模式 文字有透明度
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    auto shader = engine_->textRenderHelper_->textRenderShader_;
    shader.useShader();
    shader.setUniformMat3("transMat" , engine_->normalMatrix_);
    shader.setUniformVec4("textColor" , paint_.textColor);
    
    glBindVertexArray(vao_);
    // Logi("cmmmand" , "vbo id %d vao id %d" , vbo_ , vao_);
    glBindBuffer(GL_ARRAY_BUFFER , vbo_);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, fontTextureId_);
    shader.setUniformInt("fontTexture" , 0);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 6 * sizeof(float) , 
        reinterpret_cast<void *>(vboOffset_));
    glVertexAttribPointer(1 , 3 , GL_FLOAT , GL_FALSE , 6 * sizeof(float) , 
        reinterpret_cast<void *>(vboOffset_ + 3 * sizeof(float)));
    glDrawArrays(GL_TRIANGLES , 0 , vertexCount_);

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    glBindBuffer(GL_ARRAY_BUFFER , 0);
    glBindVertexArray(0);
}

//计算文本顶点数据
void TextRenderCommand::fillTextVertData(std::wstring &text , float left , float bottom,
        TextPaint &paint){
    auto textRenderHelper = engine_->textRenderHelper_;

    float x = left;
    float y = bottom;

    vertexCount_ = 6 * text.length();

    for(int i = 0; i < text.size() ;i++) {
        wchar_t ch = text[i];
        auto charInfoPtr = textRenderHelper->findCharInfo(ch);
        if(charInfoPtr == nullptr){
            continue;
        }

        float realWidth = charInfoPtr->width * paint.textSizeScale;
        float realHeight = charInfoPtr->height * paint.textSizeScale;

        x += realWidth + paint.gapSize;
    }//end for i;
}

void ShaderRenderCommand::setPreRenderCallback(std::function<void(void)> callback){
    preRenderCallback_ = callback;
}

void ShaderRenderCommand::putParams(Shader shader , Rect &rect){
    shader_ = shader;
    rect_ = rect;

    vertexCount_ = 4; //4个顶点
    attrCount_ = 3;//每个顶点3个属性长度

    float depth = engine_->getAndChangeDepthValue();

    int requestSize = vertexCount_ * attrCount_ * sizeof(float);
    int allocateSize = 0;
    allocatorVRam(requestSize , allocateSize);
    std::vector<float> buf(vertexCount_ * attrCount_);
    
    //v1
    buf[0 * 3 + 0] = rect.left;
    buf[0 * 3 + 1] = rect.top - rect.height;
    buf[0 * 3 + 2] = depth;

    //v2
    buf[1 * 3 + 0] = rect.left + rect.width;
    buf[1 * 3 + 1] = rect.top - rect.height;
    buf[1 * 3 + 2] = depth;

    //v3
    buf[2 * 3 + 0] = rect.left + rect.width;
    buf[2 * 3 + 1] = rect.top;
    buf[2 * 3 + 2] = depth;

    //v4
    buf[3 * 3 + 0] = rect.left;
    buf[3 * 3 + 1] = rect.top;
    buf[3 * 3 + 2] = depth;

    buildGlCommands(buf);
}

void ShaderRenderCommand::buildGlCommands(std::vector<float> &buf){
    glBindVertexArray(vao_);
    // Logi("cmd" , "vboOffset_ = %d",vboOffset_);
    glBindBuffer(GL_ARRAY_BUFFER , vbo_);
    glBufferSubData(GL_ARRAY_BUFFER , vboOffset_ ,buf.size() * sizeof(float) , buf.data());
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 3 * sizeof(float) , 
    //     reinterpret_cast<void *>(vboOffset_));
    glBindBuffer(GL_ARRAY_BUFFER , 0);
    glBindVertexArray(0);
}

void ShaderRenderCommand::fillShader(){
    if(preRenderCallback_ != nullptr){
        preRenderCallback_();
    }
}

void ShaderRenderCommand::runCommands(){
    if(shader_.programId <= 0){
        return;
    }

    //打开混合模式
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    shader_.useShader();
    shader_.setUniformMat3("transMat" , engine_->normalMatrix_);
    fillShader();

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER , vbo_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 3 * sizeof(float) , 
        reinterpret_cast<void *>(vboOffset_));
    glDrawArrays(GL_TRIANGLE_FAN , 0 , vertexCount_);
    glBindBuffer(GL_ARRAY_BUFFER , 0);
    glBindVertexArray(0);
}   

void ShapeRenderCommand::putParams(Rect &rect ,Paint &paint, ShapeType type){
    // Logi("ShapeRenderCommand" , "ShapeRenderCommand shape = %d" , type);
    switch(type){
        case ShapeCircle:
        shader_ = ShaderManager::getInstance()->getShaderByName(std::string("shape_circle"));
        break;
        case ShapeRect:
        shader_ = ShaderManager::getInstance()->getShaderByName(std::string("shape_rect"));
        break;
        default:
        shader_ = ShaderManager::getInstance()->getShaderByName(std::string("shape_rect"));
        break;
    }//end switch
    paint_ = paint;
    
    ShaderRenderCommand::putParams(shader_ , rect);
}

void ShapeRenderCommand::fillShader(){
    // Logi("shader_" , "shader_ id = %d" , shader_.programId);
    shader_.setUniformVec4("uColor" , paint_.color);
    shader_.setUniformInt("uFillStyle" , paint_.fillStyle);
    shader_.setUniformFloat("uStrokenWidth" , paint_.stokenWidth);
    shader_.setUniformFloat("uWidth" , rect_.width);
    shader_.setUniformFloat("uHeight" , rect_.height);
    shader_.setUniformVec4("uRect" , 
        glm::vec4(rect_.left , rect_.top , rect_.width , rect_.height));
    
    int viewWidth = engine_->appContext_->viewWidth_;
    int viewHeight = engine_->appContext_->viewHeight_;

    shader_.setUniformFloat("uViewWidth" , viewWidth);
    shader_.setUniformFloat("uViewHeight" , viewHeight);
}

// void RectRenderCommand::buildGlCommands(std::vector<float> &buf){
//     glBindVertexArray(vao_);
//     glBindBuffer(GL_ARRAY_BUFFER , vbo_);
//     glBufferSubData(GL_ARRAY_BUFFER , vboOffset_ ,buf.size() * sizeof(float) , buf.data());
//     glBindBuffer(GL_ARRAY_BUFFER , 0);
//     glBindVertexArray(0);
// }

void RectRenderCommand::putParams(Shader shader,Rect &rect
            , glm::mat4 &matrix ,Paint &paint){
    shader_ = shader;
    rect_ = rect;
    paint_ = paint;
    modelMat_ = matrix;

    vertexCount_ = 4; //4个顶点
    attrCount_ = 3;//每个顶点3个属性长度

    float depth = engine_->getAndChangeDepthValue();

    int requestSize = vertexCount_ * attrCount_ * sizeof(float);
    int allocateSize = 0;
    allocatorVRam(requestSize , allocateSize);
    std::vector<float> buf(vertexCount_ * attrCount_);
    
    //v1
    buf[0 * 3 + 0] = rect.left;
    buf[0 * 3 + 1] = rect.top - rect.height;
    buf[0 * 3 + 2] = depth;

    //v2
    buf[1 * 3 + 0] = rect.left + rect.width;
    buf[1 * 3 + 1] = rect.top - rect.height;
    buf[1 * 3 + 2] = depth;

    //v3
    buf[2 * 3 + 0] = rect.left + rect.width;
    buf[2 * 3 + 1] = rect.top;
    buf[2 * 3 + 2] = depth;

    //v4
    buf[3 * 3 + 0] = rect.left;
    buf[3 * 3 + 1] = rect.top;
    buf[3 * 3 + 2] = depth;

    buildGlCommands(buf);
}

void RectRenderCommand::runCommands(){
    if(shader_.programId <= 0){
        return;
    }

    //打开混合模式
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    shader_.useShader();
    shader_.setUniformMat4("modelMat" , modelMat_);
    shader_.setUniformMat3("transMat" , engine_->normalMatrix_);
    shader_.setUniformVec4("uColor" , paint_.color);
    shader_.setUniformInt("uFillStyle" , paint_.fillStyle);
    // fillShader();

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER , vbo_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 3 * sizeof(float) , 
        reinterpret_cast<void *>(vboOffset_));
    glDrawArrays(GL_TRIANGLE_FAN , 0 , vertexCount_);
    glBindBuffer(GL_ARRAY_BUFFER , 0);
    glBindVertexArray(0);
}

void LinesRenderCommand::putParams(Shader shader,std::vector<float> &points, Paint &paint){
    shader_ = shader;
    paint_ = paint;

    if(paint.stokenWidth < 1.1f){
        renderByglLines(points);
    }else{
        renderByRects(points);
    }
}

void LinesRenderCommand::renderByRects(std::vector<float> &points){
    if(points.size() <= 2){
        return;
    }

    renderByLines = false;

    vertexCount_ = (points.size()/2 - 1) * 6;
    attrCount_ = 3;

    const float depth = engine_->getAndChangeDepthValue();

    int requestSize = vertexCount_ * attrCount_ * sizeof(float);
    int allocateSize = 0;
    allocatorVRam(requestSize , allocateSize);
    if(allocateSize < requestSize){
        Logi("LinesRenderCommand" , "LinesRenderCommand renderByRects can not request vram buffer.");
        return;
    }

    std::vector<float> vertexBuf(vertexCount_ * attrCount_);

    float lineWidth = paint_.stokenWidth;
    float lineHalfWidth = lineWidth / 2.0f;

    glm::vec2 startPoint;
    glm::vec2 endPoint;
    int index = 2;
    const float angle90 = glm::radians(90.0f);

    uint32_t bufIdx = 0;
    while(index < points.size()){
        startPoint = glm::vec2(points[index - 2] , points[index -1]);
        endPoint = glm::vec2(points[index] , points[index + 1]);

        glm::vec2 dir = glm::normalize(endPoint - startPoint);
        
        glm::vec2 topDir(
             glm::cos(angle90)* dir[0] + glm::sin(angle90) * dir[1],
            -glm::sin(angle90)* dir[0] + glm::cos(angle90) * dir[1]
        );
        topDir = glm::normalize(topDir);

        glm::vec2 downDir(
            glm::cos(-angle90)* dir[0] + glm::sin(-angle90) * dir[1],
            -glm::sin(-angle90)* dir[0] + glm::cos(-angle90) * dir[1]
        );
        downDir = glm::normalize(downDir);

        // glm::vec2 topDir(0.0f , 1.0f);
        // glm::vec2 downDir(0.0f , -1.0f);
        
        glm::vec2 p1 = startPoint + topDir * lineHalfWidth;
        glm::vec2 p2 = endPoint + topDir * lineHalfWidth;
        glm::vec2 p3 = endPoint + downDir * lineHalfWidth;
        glm::vec2 p4 = startPoint + downDir * lineHalfWidth;

        vertexBuf[bufIdx++] = p1[0];
        vertexBuf[bufIdx++] = p1[1];
        vertexBuf[bufIdx++] = depth;

        vertexBuf[bufIdx++] = p4[0];
        vertexBuf[bufIdx++] = p4[1];
        vertexBuf[bufIdx++] = depth;

        vertexBuf[bufIdx++] = p2[0];
        vertexBuf[bufIdx++] = p2[1];
        vertexBuf[bufIdx++] = depth;

        vertexBuf[bufIdx++] = p4[0];
        vertexBuf[bufIdx++] = p4[1];
        vertexBuf[bufIdx++] = depth;

        vertexBuf[bufIdx++] = p3[0];
        vertexBuf[bufIdx++] = p3[1];
        vertexBuf[bufIdx++] = depth;

        vertexBuf[bufIdx++] = p2[0];
        vertexBuf[bufIdx++] = p2[1];
        vertexBuf[bufIdx++] = depth;

        index += 2;
    }//end while

    buildGlCommands(vertexBuf);
}

void LinesRenderCommand::renderByglLines(std::vector<float> &points){
    renderByLines = true;

    vertexCount_ = points.size() / 2; 
    attrCount_ = 3;

    float depth = engine_->getAndChangeDepthValue();

    int requestSize = vertexCount_ * attrCount_ * sizeof(float);
    int allocateSize = 0;
    allocatorVRam(requestSize , allocateSize);

    std::vector<float> buf(vertexCount_ * attrCount_);
    int originIdx = 0;
    for(int i = 0 ; i < vertexCount_ ; i++){
        buf[attrCount_ * i + 0] = points[originIdx++];
        buf[attrCount_ * i + 1] = points[originIdx++];
        buf[attrCount_ * i + 2] = depth;
    }//end for i;
    buildGlCommands(buf);
}

void LinesRenderCommand::runCommands(){
    if(shader_.programId <= 0){
        return;
    }

    shader_.useShader();
    shader_.setUniformMat3("transMat" , engine_->normalMatrix_);
    shader_.setUniformVec4("uColor" , paint_.color);

    glLineWidth(paint_.stokenWidth);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER , vbo_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 
        3 * sizeof(float) , 
        reinterpret_cast<void *>(vboOffset_));
    glDrawArrays(renderByLines?GL_LINE_STRIP:GL_TRIANGLES , 0 , vertexCount_);
    glBindBuffer(GL_ARRAY_BUFFER , 0);
    glBindVertexArray(0);
}


void ArcRenderCommand::putParams(Shader shader, Rect &rect 
        , float beginAngle , float endAngle , bool reverse,
        Paint &paint){
    shader_ = shader;
    rect_ = rect;
    angleRange_[0] = beginAngle;
    angleRange_[1] = endAngle;
    reverse_ = reverse;
    paint_ = paint;

    vertexCount_ = 4;
    attrCount_ = 3;

    const float depth = engine_->getAndChangeDepthValue();

    int requestSize = vertexCount_ * attrCount_ * sizeof(float);
    int allocateSize = 0;
    allocatorVRam(requestSize , allocateSize);
    if(allocateSize < requestSize){
        Logi("LinesRenderCommand" , "LinesRenderCommand renderByRects can not request vram buffer.");
        return;
    }

    std::vector<float> vertexBuf(vertexCount_ * attrCount_);
      
    //v1
    vertexBuf[0 * attrCount_ + 0] = rect_.left;
    vertexBuf[0 * attrCount_ + 1] = rect_.top - rect_.height;
    vertexBuf[0 * attrCount_ + 2] = depth;

    //v2
    vertexBuf[1 * attrCount_ + 0] = rect_.left + rect_.width;
    vertexBuf[1 * attrCount_ + 1] = rect_.top - rect_.height;
    vertexBuf[1 * attrCount_ + 2] = depth;

    //v3
    vertexBuf[2 * attrCount_ + 0] = rect_.left + rect_.width;
    vertexBuf[2 * attrCount_ + 1] = rect_.top;
    vertexBuf[2 * attrCount_ + 2] = depth;

    //v4
    vertexBuf[3 * attrCount_ + 0] = rect_.left;
    vertexBuf[3 * attrCount_ + 1] = rect_.top;
    vertexBuf[3 * attrCount_ + 2] = depth;
    
    buildGlCommands(vertexBuf);
}

void ArcRenderCommand::runCommands(){
    if(shader_.programId <= 0){
        return;
    }

    shader_.useShader();
    shader_.setUniformMat3("transMat" , engine_->normalMatrix_);
    shader_.setUniformVec4("uColor" , paint_.color);
    shader_.setUniformInt("uFillStyle" , paint_.fillStyle);
    shader_.setUniformFloat("uStrokenWidth" , paint_.stokenWidth);
    shader_.setUniformFloat("uReverse" , reverse_? 1.0f: 0.0f);
    // std::cout<< "uAngleRange  " << angleRange_[0] << "   " << angleRange_[1] << std::endl; 
    shader_.setUniformVec2("uAngleRange" , angleRange_[0] , angleRange_[1]);
    shader_.setUniformVec4("uRect" , glm::vec4(rect_.left , rect_.top , rect_.width , rect_.height));

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER , vbo_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 
        3 * sizeof(float) , 
        reinterpret_cast<void *>(vboOffset_));
    glDrawArrays(GL_TRIANGLE_FAN , 0 , vertexCount_);
    glBindBuffer(GL_ARRAY_BUFFER , 0);
    glBindVertexArray(0);
}

void CustomTextureShaderRenderCommand::putParams(Shader shader, 
    Rect &rect , unsigned int textureId){
    this->shader_ = shader;
    this->rect_ = rect;
    this->textureId_ = textureId;

    vertexCount_ = 4;
    attrCount_ = 3 + 2;

    const float depth = engine_->getAndChangeDepthValue();

    int requestSize = vertexCount_ * attrCount_ * sizeof(float);
    int allocateSize = 0;
    allocatorVRam(requestSize , allocateSize);
    if(allocateSize < requestSize){
        Logi("LinesRenderCommand" , "LinesRenderCommand renderByRects can not request vram buffer.");
        return;
    }
    std::vector<float> vertexBuf(vertexCount_ * attrCount_);

    //v1
    vertexBuf[0 * attrCount_ + 0] = rect_.left;
    vertexBuf[0 * attrCount_ + 1] = rect_.top - rect_.height;
    vertexBuf[0 * attrCount_ + 2] = depth;
    vertexBuf[0 * attrCount_ + 3] = 0.0f;
    vertexBuf[0 * attrCount_ + 4] = 0.0f;

    //v2
    vertexBuf[1 * attrCount_ + 0] = rect_.left + rect_.width;
    vertexBuf[1 * attrCount_ + 1] = rect_.top - rect_.height;
    vertexBuf[1 * attrCount_ + 2] = depth;
    vertexBuf[1 * attrCount_ + 3] = 1.0f;
    vertexBuf[1 * attrCount_ + 4] = 0.0f;

    //v3
    vertexBuf[2 * attrCount_ + 0] = rect_.left + rect_.width;
    vertexBuf[2 * attrCount_ + 1] = rect_.top;
    vertexBuf[2 * attrCount_ + 2] = depth;
    vertexBuf[2 * attrCount_ + 3] = 1.0f;
    vertexBuf[2 * attrCount_ + 4] = 1.0f;

    //v4
    vertexBuf[3 * attrCount_ + 0] = rect_.left;
    vertexBuf[3 * attrCount_ + 1] = rect_.top;
    vertexBuf[3 * attrCount_ + 2] = depth;
    vertexBuf[3 * attrCount_ + 3] = 0.0f;
    vertexBuf[3 * attrCount_ + 4] = 1.0f;

    buildGlCommands(vertexBuf);
}

void CustomTextureShaderRenderCommand::runCommands(){
    if(shader_.programId <= 0){
        return;
    }

    //打开混合模式
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId_);
    shader_.useShader();
    shader_.setUniformMat3("transMat" , engine_->normalMatrix_);
    shader_.setUniformInt("mainTexture" , 0);

    fillShader();

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER , vbo_);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , attrCount_ * sizeof(float) , 
        reinterpret_cast<void *>(vboOffset_));
    glVertexAttribPointer(1 , 2 , GL_FLOAT , GL_FALSE , attrCount_ * sizeof(float) , 
        reinterpret_cast<void *>(vboOffset_ + 3 * sizeof(float)));
    glDrawArrays(GL_TRIANGLE_FAN , 0 , vertexCount_);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER , 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}  


