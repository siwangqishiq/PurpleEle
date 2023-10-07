#pragma once

#include "common.hpp"
#include "vram.hpp"
#include <vector>
#include "shader.hpp"


class RenderEngine;
class VRamManager;

// 批处理操作
// 
class Batch{
public:
    const int VERTEX_COUNT_PER_PERMITIVE = 6;//每个图元包含的顶点数量

    virtual void begin() = 0;

    virtual void end() = 0;

    virtual void dispose() = 0;

    virtual void allocatorMemory();

    // ~Batch();
protected:
    bool isDrawing_ = false;
    unsigned int vbo_;
    unsigned int vao_;
    int vboOffset_;

    int vertexMaxCount_ = 0;
    int attrCountPerVertex_ = 0;

    int vertexCount_ = 0;

    Shader shader_;

    RenderEngine *renderEngine_;
    int index_ = 0;
    std::vector<float> vertexBuffer_;
    std::shared_ptr<VRamManager> vramManager_;
};

class ShapeBatch : public Batch{
public:
    ShapeBatch(RenderEngine *renderEngine);

    ~ShapeBatch(){
        Logi("ShapeBatch" , "~ShapeBatch descon");
    }
    
    void init();
    
    virtual void begin();

    virtual void end();

    virtual void dispose();

    void flush();

    void executeGlCommands();

    //绘制圆形
    void renderCircle(float cx , float cy , float radius , Paint &paint);

    //绘制矩形
    void renderRect(Rect &rectangle ,Paint &paint);
    
    //绘制矩形
    void renderRect(RectI rectangle ,Paint &paint){
        auto rect = rectangle.toRectF();
        renderRect(rect , paint);
    }

    //绘制椭圆
    void renderOval(Rect &rect , Paint &paint);

    //绘制圆角矩形
    void renderRoundRect(Rect &rect ,float radius , Paint &paint);

    //绘制周边带阴影的圆角矩形
    void renderBlurRoundRect(Rect &rect ,float radius ,float blur, Paint &paint);

    //绘制周边带阴影的圆形
    void renderBlurCircle(float cx , float cy , float radius,float blur, Paint &paint);

    //颜色线性插值的矩形 用于绘制矩形阴影
    void renderLinearGradientRect(Rect &rect , 
                glm::vec4 leftTopColor,
                glm::vec4 rightTopColor,
                glm::vec4 leftBottomColor,
                glm::vec4 rightBottomColor);

    //绘制周边带阴影的矩形
    void renderBlurRect(Rect &rect ,float blur, Paint &paint);
protected:

    void formatShape(ShapeType type , Rect &rect , Paint &paint , float extra = 0.0f);

    void updateVertexData(ShapeType type , Rect &rect ,Paint &paint , float extra = 0.0f);

    void putVertexAttribute(int vertexIndex ,ShapeType type, float x , float y 
            ,Rect &rect ,Paint &paint , float extra , float depth);

    void putVertexAttributeWithColor(int vertexIndex ,ShapeType type, float x , float y 
            ,Rect &rect ,Paint &paint 
            ,glm::vec4 color
            ,float depth);
};

class TextureImage;
class TextureImageRegion;

class SpriteBatch : public Batch{
public:
    SpriteBatch(RenderEngine *renderEngine);

    virtual void begin();

    virtual void end();

    virtual void dispose();

    void init();

    void flush();

    void renderImage(TextureImage &image , Rect &srcRect , Rect &dstRect);

    void renderImage(std::shared_ptr<TextureImage> pImage, Rect &srcRect , Rect &dstRect);

    void renderImage(TextureImage &image , Rect &srcRect , Rect &dstRect ,
            float cx , float cy , float angle);
    
    void renderRegionImage(TextureImageRegion &imageRegion , Rect &dstRect);

    ~SpriteBatch(){
        Logi("SpriteBatch" , "~SpriteBatch descon");
    }
private:
    int status;

    unsigned int currentTextureId_ = -1;

    void executeGlCommands();

    void doRender(unsigned int texId , float texWidth , float texHeight , 
            Rect &srcRect , Rect &dstRect,
            float cx , float cy , float angle);

    void updateVertexData(float texWidth , float texHeight, 
        Rect &srcRect , Rect &dstRect , 
        float rotateOriginX , float rotateOriginY , 
        float rotateAngle);

    void putVertexAttribute(int vertexIndex ,float x , float y , 
        float u , float v ,float cx , float cy , float angle , float depthValue);
};





