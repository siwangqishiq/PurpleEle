#include "sprite.hpp"
#include "texture.hpp"

std::shared_ptr<TextureImage> BuildImageByAsset(std::string path){
    std::shared_ptr<TextureInfo> info = TextureManager::getInstance()->acquireTexture(path);
    return std::make_shared<TextureImage>(info);
}

TextureImage::TextureImage(std::shared_ptr<TextureInfo> textureInfo){
    textureInfo_ = textureInfo;
}

int TextureImage::getWidth(){
    return textureInfo_ != nullptr? textureInfo_->width : 0;
}

int TextureImage::getHeight(){
    return textureInfo_ != nullptr? textureInfo_->height : 0;
}

unsigned int TextureImage::getTextureId(){
    return textureInfo_ != nullptr? textureInfo_->width : 0;
}
