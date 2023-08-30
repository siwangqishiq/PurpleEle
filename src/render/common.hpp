#pragma once

#include "glm/vec4.hpp"
#include <cmath>

const float FONT_DEFAULT_SIZE = 128.0f;

//
struct Point{
    float x;
    float y;

    Point(){
        x = 0.0f;
        y = 0.0f;
    }

    Point(float _x , float _y):x(_x) , y(_y){}

    /**
     * @brief 
     * 
     * @param cx 
     * @param cy 
     * @param angle rad
     */
    inline void rotate(float cx , float cy , float angle){
        
        float s = sin(angle);
        float c = cos(angle);

        float originX = x;
        float originY = y;
        // translate point back to origin:
        originX -= cx;
        originY -= cy;

        // rotate point
        float xnew = originX * c - originY * s;
        float ynew = originX * s + originY * c;

        // translate point back:
        x = xnew + cx;
        y = ynew + cy;
    }
};

enum ShapeType{
    ShapeCircle = 0,
    ShapeRect = 1,
    ShapeRoundRect = 2,
    ShapeOval = 3,
    ShapeBlurCircle = 4,
    ShapeLinearGradientRect = 5,
    ShapeBlurRect = 6,
    ShapeBlurRoundRect = 7
};

enum TextStyle{
    normal = 0,//普通
    italic = 1 //斜体
};

enum TextGravity{
    TopLeft,
    TopCenter,
    TopRight,
    BottomLeft,
    BottomCenter,
    BottomRight,
    CenterLeft,
    CenterRight,
    Center
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
    Rect(float l , float t , float w , float h){
        left = l;
        top = t;
        width = w;
        height = h;
    }
    Rect(){
        left = 0.0f;
        top = 0.0f;
        width = 0.0f;
        height = 0.0f;
    }

    inline float getRight(){
        return left + width;
    }

    inline float getBottom(){
        return top - height;
    }

    inline Point center() {
        Point p;
        p.x = (left + getRight()) / 2.0f;
        p.y = (getBottom() + top) / 2.0f;
        return p;
    }
};

//矩形
struct RectI{
    int left;
    int top;
    int width;
    int height;

    inline Rect toRectF(){
        Rect rect;
        rect.left = static_cast<float>(left);
        rect.top = static_cast<float>(top);
        rect.width = static_cast<float>(width);
        rect.height = static_cast<float>(height);
        return rect;
    }
    
    inline bool isPointInRect(float x , float y){
        return x >= left && x <= left + width && y <= top && y >= top - height;
    }
};

//文本绘制 配置
struct TextPaint{
    float textSizeScale = 1.0f;
    float gapSize = 9.0f;
    glm::vec4 textColor = glm::vec4(1.0f , 1.0f , 1.0f , 1.0f);
    TextStyle textStyle = normal;
    TextGravity textGravity = TopLeft;

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
  
