#include "sprite.hpp"
#include "texture.hpp"
#include "common.hpp"

std::shared_ptr<TextureImage> BuildImageByAsset(std::string path){
    std::shared_ptr<TextureInfo> info = TextureManager::getInstance()->acquireTexture(path , true);
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