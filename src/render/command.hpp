#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <iostream>
#include "common.hpp"
#include <vector>
#include "shader.hpp"

class RenderEngine;
struct CharInfo;

class RenderCommand{
public:
    RenderEngine *engine_;
    RenderCommand(RenderEngine *engine) : engine_(engine){}

    virtual void runCommands(){}

    bool used = false;

    unsigned int allocatorVRam(int acquireSize , int &allocateSize);
protected:
    unsigned int vbo_;
    unsigned int vao_;

    int vboOffset_;

    int vertexCount_;
    int attrCount_;
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
};

//自定义shader渲染命令
class ShaderRenderCommand : public RenderCommand {
public:
    ShaderRenderCommand(RenderEngine *engine) 
        :RenderCommand(engine){
    }

    void putParams(Shader shader, Rect &rect);
    
    virtual void runCommands();
    
    //set shader params
    virtual void fillShader();
protected:
    Shader shader_;
    Rect rect_;

    void buildGlCommands(std::vector<float> &buf);
};

class ShapeRenderCommand : public ShaderRenderCommand {
public:
    ShapeRenderCommand(RenderEngine *engine) :ShaderRenderCommand(engine){
    }

    void putParams(Rect &rect ,Paint &paint, ShapeType type);

    virtual void fillShader();
private:
    ShapeType shapeType_ = ShapeRect;
    Paint paint_;
};



