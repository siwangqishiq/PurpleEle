#include "render_batch.hpp"
#include "log.hpp"

// int Batch::vertexSize(){
//     return DEFAULT_VERTEX_SIZE;
// }

ShapeBatch::ShapeBatch(){
    isDrawing_ = false;
    
    vertexMaxCount_ = 1024;
    attrCountPerVertex_ = 3 + 3 + 1 + 3;//pos color mode rect
}

void ShapeBatch::init(){
    int requestSize = vertexMaxCount_ * attrCountPerVertex_ * sizeof(float);
    vramManager_ = std::make_shared<VRamManager>();
    
    buffer_ = std::make_shared<std::vector<float>>(vertexMaxCount_ * attrCountPerVertex_);

    int requestSizeResult = 0;
    vramManager_->fetchVideoMemory(requestSize , vbo_ ,vao_, vboOffset_ , requestSizeResult);
    Logi("ShapeBatch" , "fetch video memoroy size : %d" , requestSizeResult);
}

void ShapeBatch::begin(){
    isDrawing_ = true;
    index = 0;
}

void ShapeBatch::end(){
    flush();

    isDrawing_ = false;
    index = 0;
}

void ShapeBatch::flush(){
    //do real opengl render
}

void ShapeBatch::dispose(){
    Logi("ShapeBatch" , "dispose");
    if(vramManager_ != nullptr){
        vramManager_->clear();
    }
}

//绘制圆形
void ShapeBatch::renderCircle(float cx , float cy , float radius , Paint &paint){
    if(!isDrawing_){
        Logi("ShapeBatch" , "batch is not call begin()");
        return;
    }

    int addedSize = VERTEX_COUNT_PER_PERMITIVE * attrCountPerVertex_;
    if(index + addedSize >= buffer_->size()){
        end();
    }
}

//绘制矩形
void ShapeBatch::renderRect(Rect &rectangle ,Paint &paint){
    if(!isDrawing_){
        Logi("ShapeBatch" , "batch is not call begin()");
        return;
    }

    int addedSize = VERTEX_COUNT_PER_PERMITIVE * attrCountPerVertex_;
    if(index + addedSize >= buffer_->size()){
        end();
    }

}