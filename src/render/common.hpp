#pragma once

#include "glm/vec4.hpp"

const float FONT_DEFAULT_SIZE = 64.0f;

enum ShapeType{
    ShapeCircle = 0,
    ShapeRect = 1,
    ShapeRoundRect = 2,
    ShapeOval = 3
};

enum TextStyle{
    normal = 0,//普通
    italic = 1 //斜体
};

enum TextGravity{
    left,
    center,
    right
};

enum FillStyle{
    Filled = 0,
    Stroken = 1
};


//矩形
struct Rect{
    float left;
    float top;
    float width;
    float height;

    inline float getRight(){
        return left + width;
    }

    inline float getBottom(){
        return top - height;
    }
};

//文本绘制 配置
struct TextPaint{
    float textSizeScale = 1.0f;
    float gapSize = 4.0f;
    glm::vec4 textColor = glm::vec4(1.0f , 1.0f , 1.0f , 1.0f);
    TextStyle textStyle = normal;
    TextGravity textGravity = left;

    inline void setTextSize(float size){
        textSizeScale = (size / FONT_DEFAULT_SIZE);
    }
};

//形状绘制 配置参数
struct Paint{
    glm::vec4 color = glm::vec4(1.0f , 1.0f , 1.0f , 1.0f);
    FillStyle fillStyle = Filled;
    float stokenWidth = 1.0f;
};




