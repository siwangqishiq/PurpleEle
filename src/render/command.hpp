#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <iostream>
#include "common.hpp"
#include <vector>

class RenderEngine;
struct CharInfo;

class RenderCommand{
public:
    RenderEngine *engine_;
    RenderCommand(RenderEngine *engine) : engine_(engine){}

    virtual void runCommands(){}

    bool used = false;

protected:
    unsigned int vbo_;
    unsigned int vao_;

    int vboOffset_;

    int vertexCount_;
};

// 文本渲染命令
class TextRenderCommand : public RenderCommand{
public:
    static TextPaint defaultTextPaint;

    TextRenderCommand(RenderEngine *engine) : RenderCommand(engine) {
    }

    void putParams(std::wstring text , float left , float bottom 
                    ,TextPaint &paint = defaultTextPaint);

    void putTextParamsByRectLimit(std::wstring &text , Rect &limitRect, 
            TextPaint &paint = defaultTextPaint);

    virtual void runCommands();
    
    void setPaint(TextPaint &paint){
        paint_ = paint;
    }
private:
    TextPaint paint_;
    Rect limitRect_;
    glm::vec4 textColor_;

    const int vertCountPerChar_ = 6;//一个字符由几个顶点确定
    const int attrCount_ = 3 + 2; //每个顶点包含的属性长度     位置 postion 3 + 纹理坐标 uv 2

    unsigned int fontTextureId_ = -1;

    void buildGlCommands(std::vector<float> &buf);

    void fillTextVertData(std::wstring &text , float left , float bottom , 
            TextPaint &paint = defaultTextPaint);
    
    float findCharMaxHeight(std::wstring &text , TextPaint &paint = defaultTextPaint);
    
    float calOffsetY(std::shared_ptr<CharInfo> charInfo , float scale = 1.0f);

    float calTextStyleItalicOffset(std::shared_ptr<CharInfo> charInfo , TextPaint &paint);

    unsigned int allocatorVRamForText(int textLength);

    void putVertexDataToBuf(std::vector<float> &buf, int index, 
                            float x ,float y,
                            std::shared_ptr<CharInfo> charInfo ,
                            TextPaint &paint);
        
    // bool isSymbol(std::wstring ch);
    // std::unordered_map<wchar_t , wchar_t> loadSymbolMap(){
    //     std::unordered_map<wchar_t , wchar_t> symbolMap;
    //     for(wchar_t ch : symbolStr){
    //         symbolMap[ch] = ch;
    //     }
    //     return symbolMap;
    // }
    // std::unordered_map<wchar_t , wchar_t> symbolMap_;
    // std::wstring symbolStr = 
    // L"1234567890!@#$%^&*()_[]{};,.<>/?abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ，";
};



