#pragma once

#include "common.hpp"
#include "vram.hpp"
#include <vector>

// 批处理操作
// 
class Batch{
public:
    virtual void begin() = 0;

    virtual void end() = 0;

    virtual void dispose() = 0;
protected:
    bool isDrawing_ = false;
    unsigned int vbo_;
    unsigned int vao_;
    int vboOffset_;

    int vertexMaxCount_ = 0;
    int vertexAttrCount_ = 0;
};

class VRamManager;

class ShapeBatch : public Batch{
public:
    ShapeBatch();
    
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
    int index = 0;
    
    std::shared_ptr<std::vector<float>> buffer_;

private:
    std::shared_ptr<VRamManager> vramManager_;
};





