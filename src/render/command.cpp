#include "command.hpp"
#include "render.hpp"
#include "log.hpp"
#include "vram.hpp"
#include "../../libjson/json.hpp"
#include "common.hpp"


TextPaint TextRenderCommand::defaultTextPaint;

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
    if(charInfo == nullptr || isSymbol(charInfo->value)){
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

bool TextRenderCommand::isSymbol(std::wstring ch){
    auto charMap = loadSymbolMap();
    //Logi("issymbol" , "%d symbolMap %s" ,ch.empty(), ToByteString(ch).c_str());
    //std::cout << "xx : " << (symbolMap_.find(ch[0]) != symbolMap_.end()) << std::endl;
    return charMap.find(ch[0]) != charMap.end();
}

unsigned int TextRenderCommand::allocatorVRamForText(int textLength){
    vertexCount_ = vertCountPerChar_ * textLength;
    int requestSize = vertexCount_ * attrCount_ * sizeof(float);
    int allocateSize = 0;
    // Logi("command" , "allocator size = %d" , requestSize);
    VRamManager::getInstance().fetchVideoMemory(requestSize , 
        vbo_ ,vao_, vboOffset_ , allocateSize);
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

    auto t2 = currentTimeMillis();

    auto textRenderHelper = engine_->textRenderHelper_;
    std::vector<float> buf(vertexCount_ * attrCount_);

    for(int i = 0 ; i < text.length() ;i++){
        wchar_t ch = text[i];
        auto charInfoPtr = textRenderHelper->findCharInfo(ch);
        if(charInfoPtr == nullptr){
            continue;
        }
        
        putVertexDataToBuf(buf , i , x , y , charInfoPtr , paint);
        x += charInfoPtr->width * paint.textSizeScale + paint.gapSize;
    }//end for i
    Logi("TextRenderCommand" , "allocatorVRamForText layout vertex : %lld" , (currentTimeMillis() - t2));

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
    fontTextureId_ = engine_->textRenderHelper_->mainTextureId_;
    allocatorVRamForText(text.length());

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

    //eg: 一 need a offset in y ax
    float offsetY = calOffsetY(charInfoPtr , sizeScale);
    float offsetX = 0.0f;

    int offset = index * attrPerChar;
    //v1
    buf[offset + 0] = x + offsetX;
    buf[offset + 1] = y + offsetY;
    buf[offset + 2] = 1.0f;
    buf[offset + 3] = texLeft;
    buf[offset + 4] = texBottom;
    
    //v2
    buf[offset + 5] = x + charRealWidth + offsetX;
    buf[offset + 6] = y + offsetY;
    buf[offset + 7] = 1.0f;
    buf[offset + 8] = texRight;
    buf[offset + 9] = texBottom;

    //v3
    buf[offset + 10] = x + charRealWidth + offsetX + italicOffset;
    buf[offset + 11] = y + offsetY + charRealHeight;
    buf[offset + 12] = 1.0f;
    buf[offset + 13] = texRight;
    buf[offset + 14] = texTop;
    
    //v4
    buf[offset + 15] = x + offsetX;
    buf[offset + 16] = y + offsetY;
    buf[offset + 17] = 1.0f;
    buf[offset + 18] = texLeft;
    buf[offset + 19] = texBottom;

    //v5
    buf[offset + 20] = x + charRealWidth + offsetX + italicOffset;
    buf[offset + 21] = y + offsetY + charRealHeight;
    buf[offset + 22] = 1.0f;
    buf[offset + 23] = texRight;
    buf[offset + 24] = texTop;

    //v6
    buf[offset + 25] = x + offsetX + italicOffset;
    buf[offset + 26] = y + offsetY + charRealHeight;
    buf[offset + 27] = 1.0f;
    buf[offset + 28] = texLeft;
    buf[offset + 29] = texTop;
}


void TextRenderCommand::buildGlCommands(std::vector<float> &buf){
    glBindVertexArray(vao_);
    // Logi("cmd" , "vboOffset_ = %d",vboOffset_);
    glBindBuffer(GL_ARRAY_BUFFER , vbo_);
    glBufferSubData(GL_ARRAY_BUFFER , vboOffset_ , 
        buf.size() * sizeof(float) , buf.data());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 5 * sizeof(float) , 
        reinterpret_cast<void *>(vboOffset_));
    glVertexAttribPointer(1 , 2 , GL_FLOAT , GL_FALSE , 5 * sizeof(float) , 
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
    glBindTexture(GL_TEXTURE_2D, fontTextureId_);
    shader.setUniformInt("fontTexture" , 0);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 5 * sizeof(float) , 
        reinterpret_cast<void *>(vboOffset_));
    glVertexAttribPointer(1 , 2 , GL_FLOAT , GL_FALSE , 5 * sizeof(float) , 
        reinterpret_cast<void *>(vboOffset_ + 3 * sizeof(float)));
    glDrawArrays(GL_TRIANGLES , 0 , vertexCount_);

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