#pragma once

#include "common.hpp"
#include "vram.hpp"
#include <vector>
#include "shader.hpp"

// 批处理操作
// 
class Batch{
public:
    virtual void begin() = 0;

    virtual void end() = 0;

    virtual void dispose() = 0;

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
};

class RenderEngine;
class VRamManager;

class ShapeBatch : public Batch{
public:
    const int VERTEX_COUNT_PER_PERMITIVE = 6;//每个图元包含的顶点数量

    // ShapeBatch(std::shared_ptr<RenderEngine> renderEngine);

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

protected:
    RenderEngine *renderEngine_;

    int index_ = 0;
    std::vector<float> vertexBuffer_;
    std::shared_ptr<VRamManager> vramManager_;

    void updateVertexData(ShapeType type , Rect &rect ,Paint &paint);

    void putVertexAttribute(int vertexIndex ,ShapeType type, float x , float y 
            ,Rect &rect ,Paint &paint);
};





