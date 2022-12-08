#pragma once

#include "glm/vec4.hpp"

const float FONT_DEFAULT_SIZE = 64.0f;

enum TextStyle{
    normal = 0,//普通
    italic = 1 //斜体
};

enum TextGravity{
    left,
    center,
    right
};

//文本绘制 配置
struct TextPaint{
    float textSizeScale = 1.0f;
    float gapSize = 4.0f;
    glm::vec4 textColor = glm::vec4(1.0f , 1.0f , 1.0f , 1.0f);
    TextStyle textStyle = normal;
    TextGravity textGravity = left;
};

//矩形
struct Rect{
    float left;
    float top;
    float width;
    float height;
};



