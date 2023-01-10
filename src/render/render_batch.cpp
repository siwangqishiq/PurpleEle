#include "render_batch.hpp"
#include "log.hpp"
#include "render.hpp"
#include "sprite.hpp"

// int Batch::vertexSize(){
//     return DEFAULT_VERTEX_SIZE;
// }

// Batch::~Batch(){
//     Logi("Batch" , "~batch deconstructor");
// }

void Batch::allocatorMemory(){
    int requestSize = vertexMaxCount_ * attrCountPerVertex_ * sizeof(float);
    vramManager_ = std::make_shared<VRamManager>();

    vertexBuffer_ = std::vector<float>(vertexMaxCount_ * attrCountPerVertex_);

    int requestSizeResult = 0;
    vramManager_->fetchVideoMemory(requestSize , vbo_ ,vao_, vboOffset_ , requestSizeResult);
    Logi("ShapeBatch" , "fetch video memoroy size : %d" , requestSizeResult);
}

ShapeBatch::ShapeBatch(RenderEngine *renderEngine){
    renderEngine_ = renderEngine;
    isDrawing_ = false;
    
    vertexMaxCount_ = 1024;
    attrCountPerVertex_ = 3 + 4 + 4 + 4;//pos + color + shape + rect
}

void ShapeBatch::init(){
    allocatorMemory();

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER , vbo_);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , attrCountPerVertex_ * sizeof(float) ,
        reinterpret_cast<void *>(vboOffset_ + 0));
    glVertexAttribPointer(1 , 4 , GL_FLOAT , GL_FALSE , attrCountPerVertex_ * sizeof(float) ,
                          reinterpret_cast<void *>(vboOffset_ + 3* sizeof(float)));
    glVertexAttribPointer(2 , 4 , GL_FLOAT , GL_FALSE , attrCountPerVertex_ * sizeof(float) ,
                          reinterpret_cast<void *>(vboOffset_ + (3 + 4)* sizeof(float)));
    glVertexAttribPointer(3 , 4 , GL_FLOAT , GL_FALSE , attrCountPerVertex_ * sizeof(float) ,
                          reinterpret_cast<void *>(vboOffset_ + (3 + 4 + 4)* sizeof(float)));
    glBindVertexArray(0);

    shader_ = ShaderManager::getInstance()->loadAssetShader("shape_batch_render",
                    "shader/shape_batch_vertex.glsl", "shader/shape_batch_frag.glsl");
}

void ShapeBatch::begin(){
    isDrawing_ = true;
    index_ = 0;
}

void ShapeBatch::end(){
    flush();

    isDrawing_ = false;
    index_ = 0;
    vertexCount_ = 0;
}

void ShapeBatch::flush(){
    //do real opengul render
    executeGlCommands();
}

void ShapeBatch::executeGlCommands(){
    if(index_ <= 0){
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER , vbo_);
    glBufferSubData(GL_ARRAY_BUFFER , vboOffset_ ,
                    index_ * sizeof(float) , vertexBuffer_.data());
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
    Rect rect;
    rect.left = cx - radius;
    rect.top = cy + radius;
    rect.width = 2 * radius;
    rect.height = 2 * radius;
    formatShape(ShapeType::ShapeCircle , rect , paint);
}

//绘制矩形
void ShapeBatch::renderRect(Rect &rect ,Paint &paint){
    formatShape(ShapeType::ShapeRect , rect , paint);
}

void ShapeBatch::renderOval(Rect &rect , Paint &paint){
    formatShape(ShapeType::ShapeOval , rect , paint);
}

void ShapeBatch::renderRoundRect(Rect &rect ,float radius , Paint &paint){
    formatShape(ShapeType::ShapeRoundRect , rect , paint , radius);
}

void ShapeBatch::formatShape(ShapeType type , Rect &rect , Paint &paint , float extra){
    if(!isDrawing_){
        Logi("ShapeBatch" , "batch is not call begin()");
        return;
    }

    int addedSize = VERTEX_COUNT_PER_PERMITIVE * attrCountPerVertex_;
    if(index_ + addedSize >= vertexBuffer_.size()){
        end();
        begin();
    }

    updateVertexData(type , rect , paint , extra);
}

void ShapeBatch::updateVertexData(ShapeType type ,Rect &rect ,Paint &paint , float extra){
    //v1
    putVertexAttribute(0 , type, rect.left , rect.getBottom() , rect, paint , extra);

    //v2
    putVertexAttribute(1 ,type, rect.getRight() , rect.getBottom(), rect , paint ,extra);

    //v3
    putVertexAttribute(2 ,type , rect.getRight() , rect.top, rect , paint , extra);

    //v4
    putVertexAttribute(3 ,type , rect.left , rect.getBottom(), rect , paint , extra);

    //v5
    putVertexAttribute(4 ,type , rect.getRight() , rect.top, rect , paint , extra);

    //v6
    putVertexAttribute(5 ,type , rect.left , rect.top, rect , paint , extra);

    index_ += attrCountPerVertex_ * VERTEX_COUNT_PER_PERMITIVE;
    vertexCount_ += VERTEX_COUNT_PER_PERMITIVE;
}

void ShapeBatch::putVertexAttribute(int vertexIndex 
            ,ShapeType type ,float x , float y 
            ,Rect &rect ,Paint &paint , float extra){
    const int offset = index_ + vertexIndex * attrCountPerVertex_;
    //position
    vertexBuffer_[offset + 0] = x;
    vertexBuffer_[offset + 1] = y;
    vertexBuffer_[offset + 2] = 1.0f;

    //color
    vertexBuffer_[offset + 3] = paint.color.r;
    vertexBuffer_[offset + 4] = paint.color.g;
    vertexBuffer_[offset + 5] = paint.color.b;
    vertexBuffer_[offset + 6] = paint.color.a;

    //shape
    vertexBuffer_[offset + 7] = type;
    vertexBuffer_[offset + 8] = paint.fillStyle;
    vertexBuffer_[offset + 9] = paint.stokenWidth;
    vertexBuffer_[offset + 10] = extra; //if shape roundrect this is round rect radius

    //rect
    vertexBuffer_[offset + 11] = rect.left;
    vertexBuffer_[offset + 12] = rect.top;
    vertexBuffer_[offset + 13] = rect.width;
    vertexBuffer_[offset + 14] = rect.height;
}

SpriteBatch::SpriteBatch(RenderEngine *engine){
    renderEngine_ = engine;

    isDrawing_ = false;

    vertexMaxCount_ = 4 * 1024; //4K
    attrCountPerVertex_ = 3 + 2;//pos + uv
}

void SpriteBatch::init(){
    allocatorMemory();
    
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER , vbo_);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 
        attrCountPerVertex_ * sizeof(float) ,reinterpret_cast<void *>(vboOffset_ + 0));
    glVertexAttribPointer(1 , 2 , GL_FLOAT , GL_FALSE , 
        attrCountPerVertex_ * sizeof(float) ,reinterpret_cast<void *>(vboOffset_ + 3* sizeof(float)));

    glBindVertexArray(0);

    shader_ = ShaderManager::getInstance()->loadAssetShader("sprite_batch_render",
                    "shader/sprite_batch_vertex.glsl", "shader/sprite_batch_frag.glsl");
}

void SpriteBatch::begin(){
    isDrawing_ = true;
    index_ = 0;
}

void SpriteBatch::end(){
    flush();

    isDrawing_ = false;
    index_ = 0;
    vertexCount_ = 0;
}

void SpriteBatch::dispose(){
    Logi("SpriteBatch" , "dispose");
    if(vramManager_ != nullptr){
        vramManager_->clear();
    }
}

void SpriteBatch::flush(){
    executeGlCommands();
}

void SpriteBatch::executeGlCommands(){
    if(index_ <= 0){
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER , vbo_);
    glBufferSubData(GL_ARRAY_BUFFER , vboOffset_ ,
                    index_ * sizeof(float) , vertexBuffer_.data());
    glBindBuffer(GL_ARRAY_BUFFER , 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    shader_.useShader();
    shader_.setUniformMat3("transMat" , renderEngine_->normalMatrix_);

    glBindVertexArray(vao_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D , currentTextureId_);
    glDrawArrays(GL_TRIANGLES , 0 , vertexCount_);
    shader_.setUniformInt("uTex",0);
    
    glBindTexture(GL_TEXTURE_2D , 0);
    glBindVertexArray(0);
}

void SpriteBatch::renderImage(Image &image , Rect &srcRect , Rect &dstRect){
    if(!isDrawing_){
        Logi("SpriteBatch" , "batch is not call begin()");
        return;
    }

    if(image.getTextureId() != currentTextureId_){
        end();
        currentTextureId_ = image.getTextureId();
        begin();
    }

    int addedSize = VERTEX_COUNT_PER_PERMITIVE * attrCountPerVertex_;
    if(index_ + addedSize >= vertexBuffer_.size()){
        end();
        begin();
    }

    updateVertexData(image , srcRect , dstRect);
}

void SpriteBatch::updateVertexData(Image &image ,Rect &srcRect , Rect &dstRect){
    //v1
    putVertexAttribute(0 , 
        dstRect.left , dstRect.getBottom(), 
        srcRect.left / (float)image.getWidth() , 
        srcRect.getBottom() / (float)image.getHeight());

    //v2
    putVertexAttribute(1, dstRect.getRight() , dstRect.getBottom(), 
        srcRect.getRight() / (float)image.getWidth() , 
        srcRect.getBottom() / (float)image.getHeight());

    //v3
    putVertexAttribute(2, dstRect.getRight() , dstRect.top, 
        srcRect.getRight() / (float)image.getWidth() , 
        srcRect.top / (float)image.getHeight());

    //v4
    putVertexAttribute(3, dstRect.left , dstRect.getBottom(), 
        srcRect.left / (float)image.getWidth() , 
        srcRect.getBottom() / (float)image.getHeight());

    //v5
    putVertexAttribute(4, dstRect.getRight() , dstRect.top, 
        srcRect.getRight() / (float)image.getWidth() , 
        srcRect.top / (float)image.getHeight());

    //v6
    putVertexAttribute(5, dstRect.left , dstRect.top, 
        srcRect.left / (float)image.getWidth() ,
        srcRect.top / (float)image.getHeight());

    index_ += attrCountPerVertex_ * VERTEX_COUNT_PER_PERMITIVE;
    vertexCount_ += VERTEX_COUNT_PER_PERMITIVE;
}

void SpriteBatch::putVertexAttribute(int vertexIndex ,float x , float y , 
        float u , float v){
    const int offset = index_ + vertexIndex * attrCountPerVertex_;

    //position
    vertexBuffer_[offset + 0] = x;
    vertexBuffer_[offset + 1] = y;
    vertexBuffer_[offset + 2] = 1.0f;

    //uv
    vertexBuffer_[offset + 3] = u;
    vertexBuffer_[offset + 4] = v;
}