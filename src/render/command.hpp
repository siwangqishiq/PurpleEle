#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <iostream>
#include "common.hpp"
#include <vector>
#include "shader.hpp"
#include <functional>

class RenderEngine;
struct CharInfo;

class RenderCommand{
public:
    RenderEngine *engine_;
    RenderCommand(RenderEngine *engine) : engine_(engine){}

    virtual void runCommands(){}

    bool used = false;

    unsigned int allocatorVRam(int acquireSize , int &allocateSize);

    int vertexCount_;
    int attrCount_;
protected:
    unsigned int vbo_;
    unsigned int vao_;

    int vboOffset_;
};

// 文本渲染命令
class TextRenderCommand : public RenderCommand{
public:
    const int vertCountPerChar_ = 6;//一个字符由几个顶点确定
    static TextPaint defaultTextPaint;

    TextRenderCommand(RenderEngine *engine) : RenderCommand(engine) {
    }

    void putParams(std::wstring text , float left , float bottom 
                    ,TextPaint &paint = defaultTextPaint);

    void putTextParamsByRectLimit(std::wstring &text , Rect &limitRect, 
            Rect *wrapContentRect,
            TextPaint &paint = defaultTextPaint);

    virtual void runCommands();
    
    void setPaint(TextPaint &paint){
        paint_ = paint;
    }

    TextPaint paint_;

    Rect limitRect_;

    void putVertexDataToBuf(std::vector<float> &buf, int index, 
                            float x ,float y,float depth,
                            std::shared_ptr<CharInfo> charInfo ,
                            TextPaint &paint);
    
    void updateVertexPositionData(std::vector<float> &buf, int index, 
                            float x ,float y);
private:
    
    glm::vec4 textColor_;


    //每个顶点包含的属性长度 
    //位置 postion 3 + 纹理坐标 uvw 3
    const int attrCount_ = 3 + 3; 
    
    unsigned int fontTextureId_ = -1;

    void buildGlCommands(std::vector<float> &buf);

    void fillTextVertData(std::wstring &text , float left , float bottom , 
            TextPaint &paint = defaultTextPaint);
    
    float findCharMaxHeight(std::wstring &text , TextPaint &paint = defaultTextPaint);
    
    float calOffsetY(std::shared_ptr<CharInfo> charInfo , float scale = 1.0f);

    float calTextStyleItalicOffset(std::shared_ptr<CharInfo> charInfo , TextPaint &paint);

    unsigned int allocatorVRamForText(int textLength);
};

//自定义shader渲染命令
class ShaderRenderCommand : public RenderCommand {
public:
    ShaderRenderCommand(RenderEngine *engine) 
        :RenderCommand(engine){
    }

    void putParams(Shader shader, Rect &rect);

    void setPreRenderCallback(std::function<void(void)> callback);
    
    virtual void runCommands();
    
    //set shader params
    virtual void fillShader();
protected:
    Shader shader_;
    Rect rect_;

    std::function<void(void)> preRenderCallback_ = nullptr;

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

class RectRenderCommand: public RenderCommand{
public:
    RectRenderCommand(RenderEngine *engine) 
        :RenderCommand(engine){
    }

    virtual void putParams(Shader shader, Rect &rect ,glm::mat4 &matrix
        ,Paint &paint);

    virtual void runCommands() override;
protected:
    void buildGlCommands(std::vector<float> &buf);
private:
    Shader shader_;
    Rect rect_;
    Paint paint_;

    glm::mat4 modelMat_;
};

