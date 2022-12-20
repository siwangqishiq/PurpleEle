#include "render_batch.hpp"
#include "log.hpp"
#include "render.hpp"

// int Batch::vertexSize(){
//     return DEFAULT_VERTEX_SIZE;
// }

// Batch::~Batch(){
//     Logi("Batch" , "~batch deconstructor");
// }

ShapeBatch::ShapeBatch(RenderEngine *renderEngine){
    renderEngine_ = renderEngine;
    isDrawing_ = false;
    
    vertexMaxCount_ = 1024;
    attrCountPerVertex_ = 3;//pos
    // attrCountPerVertex_ = 3 + 3 + 1 + 3;//pos + color + mode + rect
}

void ShapeBatch::init(){
    int requestSize = vertexMaxCount_ * attrCountPerVertex_ * sizeof(float);
    vramManager_ = std::make_shared<VRamManager>();

    vertexBuffer_ = std::vector<float>(vertexMaxCount_ * attrCountPerVertex_);

    int requestSizeResult = 0;
    vramManager_->fetchVideoMemory(requestSize , vbo_ ,vao_, vboOffset_ , requestSizeResult);
    Logi("ShapeBatch" , "fetch video memoroy size : %d" , requestSizeResult);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER , vbo_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 3 * sizeof(float) , 
        reinterpret_cast<void *>(vboOffset_));
    glBindVertexArray(0);

    shader_ = ShaderManager::getInstance()->loadAssetShader("shape_batch_render",
                    "shader/shape_batch_vertex.glsl", "shader/shape_batch_frag.glsl");
}

void ShapeBatch::begin(){
    isDrawing_ = true;
    index = 0;
}

void ShapeBatch::end(){
    flush();

    isDrawing_ = false;
    index = 0;
    vertexCount_ = 0;
}

void ShapeBatch::flush(){
    //do real opengul render
    executeGlCommands();
}

void ShapeBatch::executeGlCommands(){
    if(index <= 0){
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER , vbo_);
    glBufferSubData(GL_ARRAY_BUFFER , vboOffset_ , 
        index * sizeof(float) , vertexBuffer_.data());
    glBindBuffer(GL_ARRAY_BUFFER , 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    // Logi("ShapeBatch" , "vertexBuffer_ vboOffset_ = %d" , vboOffset_);
    shader_.useShader();
    shader_.setUniformMat3("transMat" , renderEngine_->normalMatrix_);
    
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES , 0 , vertexCount_);
    glBindVertexArray(0);
}

void ShapeBatch::dispose(){
    Logi("ShapeBatch" , "dispose");
    if(vramManager_ != nullptr){
        vramManager_->clear();
    }
}

//绘制圆形
void ShapeBatch::renderCircle(float cx , float cy , float radius , Paint &paint){
}

//绘制矩形
void ShapeBatch::renderRect(Rect &rectangle ,Paint &paint){
    if(!isDrawing_){
        Logi("ShapeBatch" , "batch is not call begin()");
        return;
    }

    int addedSize = VERTEX_COUNT_PER_PERMITIVE * attrCountPerVertex_;
    if(index + addedSize >= vertexBuffer_.size()){
        end();
        begin();
    }

    updateVertexData(rectangle , paint);
}

void ShapeBatch::updateVertexData(Rect &rect ,Paint &paint){
    //v1
    vertexBuffer_[index + 0] = rect.left;
    vertexBuffer_[index + 1] = rect.getBottom();
    vertexBuffer_[index + 2] = 1.0f;

    //v2
    vertexBuffer_[index + 3] = rect.getRight();
    vertexBuffer_[index + 4] = rect.getBottom();
    vertexBuffer_[index + 5] = 1.0f;

    //v3
    vertexBuffer_[index + 6] = rect.getRight();
    vertexBuffer_[index + 7] = rect.top;
    vertexBuffer_[index + 8] = 1.0f;

    //v4
    vertexBuffer_[index + 9] = rect.left;
    vertexBuffer_[index + 10] = rect.getBottom();
    vertexBuffer_[index + 11] = 1.0f;

    //v5
    vertexBuffer_[index + 12] = rect.getRight();
    vertexBuffer_[index + 13] = rect.top;
    vertexBuffer_[index + 14] = 1.0f;

    //v6
    vertexBuffer_[index + 15] = rect.left;
    vertexBuffer_[index + 16] = rect.top;
    vertexBuffer_[index + 17] = 1.0f;

    index += attrCountPerVertex_ * VERTEX_COUNT_PER_PERMITIVE;
    vertexCount_ += VERTEX_COUNT_PER_PERMITIVE;

    // for(int i = 0 ; i < index ;i++){
    //     std::cout << vertexBuffer_[i] << "  ";
    // }
    // std::cout << std::endl;
}