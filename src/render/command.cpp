#include "command.hpp"
#include "render.hpp"
#include "log.hpp"
#include "vram.hpp"
#include "../../libjson/json.hpp"
#include "common.hpp"
#include "application.hpp"

TextPaint TextRenderCommand::defaultTextPaint;

unsigned int RenderCommand::allocatorVRam(int acquireSize , int &allocateSize){
    VRamManager::getInstance()->fetchVideoMemory(acquireSize , vbo_ ,vao_, vboOffset_ , allocateSize);
    return vbo_;
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

    for(int i = 0 ; i < text.length() ;i++){
        wchar_t ch = text[i];
        auto charInfoPtr = textRenderHelper->findCharInfo(ch);
        if(charInfoPtr == nullptr){
            Logi("TextRenderCommand" , "not found char info");
            continue;
        }
        
        // Logi("TextRenderCommand" , "%f %f" 
        //     , charInfoPtr->textureCoords[2],charInfoPtr->textureCoords[3]);
        putVertexDataToBuf(buf , i , x , y , charInfoPtr , paint);
        x += (charInfoPtr->width + paint.gapSize) * paint.textSizeScale;
    }//end for i
    // Logi("TextRenderCommand" , "layout vertex vertexcount : %d , costtime =  %lld" 
    //     , vertexCount_
    //     , (currentTimeMicro() - t2));

    // auto t3 = currentTimeMillis();
    buildGlCommands(buf);
    // Logi("TextRenderCommand" , "buildGlCommands time : %lld" , (currentTimeMillis() - t3));
}

void TextRenderCommand::putTextParamsByRectLimit(std::wstring &text , Rect &limitRect, 
            TextPaint &paint){
    if(text.empty()){
        return;
    }
    paint_ = paint;
    limitRect_ = limitRect;
    fontTextureId_ = engine_->textRenderHelper_->mainTextureId_;
    allocatorVRamForText(text.length());
    
    long long t2 = currentTimeMicro();

    auto textRenderHelper = engine_->textRenderHelper_;
    //todo 文本排版
    const float limitWidth = limitRect.width;
    const float sizeScale = paint_.textSizeScale;
    const float charMaxHeight = CHAR_DEFAULT_HEIGHT * sizeScale;

    int pos = 0;

    float x = limitRect.left;
    float y = limitRect.top - charMaxHeight;
    float thisLineLeft = 0.0f;

    std::vector<float> buf(vertexCount_ * attrCount_);
    while(pos < text.size()){
        auto ch = text[pos];
        auto charInfoPtr = textRenderHelper->findCharInfo(ch);
        if(charInfoPtr != nullptr){
            float charRealWidth = charInfoPtr->width * sizeScale;
            if(x + (charRealWidth + paint.gapSize) > limitRect.left + limitWidth){
                x = limitRect.left;
                y -= (charMaxHeight + paint.gapSize);
            }
            putVertexDataToBuf(buf , pos , x , y , charInfoPtr , paint);
            x += (charRealWidth + paint.gapSize);
        }
        pos++;
    }//end while

    // Logi("TextRenderCommand" , "layout vertex vertexcount : %d , costtime =  %lld" 
    //     , vertexCount_
    //     , (currentTimeMicro() - t2));
    
    buildGlCommands(buf);
}

void TextRenderCommand::putVertexDataToBuf(std::vector<float> &buf, 
        int index,float x ,float y,
        std::shared_ptr<CharInfo> charInfoPtr ,TextPaint &paint){
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
    buf[offset + 2] = 1.0f;
    buf[offset + 3] = texLeft;
    buf[offset + 4] = texBottom;
    buf[offset + 5] = texW;
    
    //v2
    buf[offset + 6] = x + charRealWidth + offsetX;
    buf[offset + 7] = y + offsetY;
    buf[offset + 8] = 1.0f;
    buf[offset + 9] = texRight;
    buf[offset + 10] = texBottom;
    buf[offset + 11] = texW;

    //v3
    buf[offset + 12] = x + charRealWidth + offsetX + italicOffset;
    buf[offset + 13] = y + offsetY + charRealHeight;
    buf[offset + 14] = 1.0f;
    buf[offset + 15] = texRight;
    buf[offset + 16] = texTop;
    buf[offset + 17] = texW;
    
    //v4
    buf[offset + 18] = x + offsetX;
    buf[offset + 19] = y + offsetY;
    buf[offset + 20] = 1.0f;
    buf[offset + 21] = texLeft;
    buf[offset + 22] = texBottom;
    buf[offset + 23] = texW;

    //v5
    buf[offset + 24] = x + charRealWidth + offsetX + italicOffset;
    buf[offset + 25] = y + offsetY + charRealHeight;
    buf[offset + 26] = 1.0f;
    buf[offset + 27] = texRight;
    buf[offset + 28] = texTop;
    buf[offset + 29] = texW;

    //v6
    buf[offset + 30] = x + offsetX + italicOffset;
    buf[offset + 31] = y + offsetY + charRealHeight;
    buf[offset + 32] = 1.0f;
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

    int requestSize = vertexCount_ * attrCount_ * sizeof(float);
    int allocateSize = 0;
    allocatorVRam(requestSize , allocateSize);
    std::vector<float> buf(vertexCount_ * attrCount_);
    
    //v1
    buf[0 * 3 + 0] = rect.left;
    buf[0 * 3 + 1] = rect.top - rect.height;
    buf[0 * 3 + 2] = 1.0f;

    //v2
    buf[1 * 3 + 0] = rect.left + rect.width;
    buf[1 * 3 + 1] = rect.top - rect.height;
    buf[1 * 3 + 2] = 1.0f;

    //v3
    buf[2 * 3 + 0] = rect.left + rect.width;
    buf[2 * 3 + 1] = rect.top;
    buf[2 * 3 + 2] = 1.0f;

    //v4
    buf[3 * 3 + 0] = rect.left;
    buf[3 * 3 + 1] = rect.top;
    buf[3 * 3 + 2] = 1.0f;

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