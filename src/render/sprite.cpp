#include "sprite.hpp"
#include "texture.hpp"
#include "common.hpp"

std::shared_ptr<TextureImage> BuildImageByAsset(std::string path){
    std::shared_ptr<TextureInfo> info = TextureManager::getInstance()->acquireTexture(path , true);
    return std::make_shared<TextureImage>(info);
}

std::shared_ptr<TextureImage> BuildEmptyImage(int width , int height , int format){
    static int emptyTextureCount = 0;
    emptyTextureCount++;

    std::string name = std::string("empty_image") 
        + std::to_string(emptyTextureCount) 
        + std::to_string(width)+std::string("x")
        + std::to_string(height);
    auto info = TextureManager::getInstance()->createEmptyTexture(name , width , height , format);
    return std::make_shared<TextureImage>(info);
}

std::shared_ptr<TextureImageRegion> TextureImage::createImageRegion(float offsetX, float offsetY, float regionWidth , float regionHeight){
    return std::make_shared<TextureImageRegion>(getTextureId(),
        offsetX , offsetY , regionWidth , regionHeight , getWidth() , getHeight());
}

TextureImage::TextureImage(std::shared_ptr<TextureInfo> textureInfo){
    textureInfo_ = textureInfo;
}

Rect TextureImage::getRect(){
    Rect rect;
    rect.left = 0.0f;
    rect.top = getHeight();
    rect.width = getWidth();
    rect.height = getHeight();
    return rect;
}

int TextureImage::getWidth(){
    return textureInfo_ != nullptr? textureInfo_->width : 0;
}

int TextureImage::getHeight(){
    return textureInfo_ != nullptr? textureInfo_->height : 0;
}

void TextureImage::dispose(){
    if(textureInfo_ == nullptr){
        return;
    }
    
    TextureManager::getInstance()->freeTexture(*textureInfo_);
}

 void TextureImage::updateTextureData(uint8_t *pData){
    glBindTexture(GL_TEXTURE_2D , getTextureId());
    glTexSubImage2D(GL_TEXTURE_2D , 
        0 , 0,  0,
        getWidth() , 
        getHeight(), 
        GL_RED , GL_UNSIGNED_BYTE, pData);
    glBindTexture(GL_TEXTURE_2D , 0);
 }

unsigned int TextureImage::getTextureId(){
    return textureInfo_ != nullptr? textureInfo_->textureId : 0;
}

Point TextureImageRegion::getOffset(){
    return Point(offsetX_ , offsetY_);
}

float TextureImageRegion::getWidth(){
    return width_;
}

float TextureImageRegion::getHeight(){
    return height_;
}

float TextureImageRegion::getTexWidth(){
    return texWidth_;
}

float TextureImageRegion::getTexHeight(){
    return texHeight_;
}

unsigned int TextureImageRegion::getTextureId(){
    return textureId_;
}