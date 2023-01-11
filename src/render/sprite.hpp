#pragma once

#include <memory>
#include <string>

// class Image{
// public:
//     virtual int getWidth() = 0;
//     virtual int getHeight() = 0;
//     virtual unsigned int getTextureId() = 0;
// };

class TextureInfo;
class TextureImageRegion;

class TextureImage{
public:
    TextureImage(std::shared_ptr<TextureInfo> textureInfo);

    virtual int getWidth();
    virtual int getHeight();
    virtual unsigned int getTextureId();

    std::shared_ptr<TextureImageRegion> createImageRegion(float offsetX , float offsetY,
        float regionWidth , float regionHeight);

private:
    std::shared_ptr<TextureInfo> textureInfo_;
};

class Point;

class TextureImageRegion{
public:
    Point getOffset();
    float getWidth();
    float getHeight();
    float getTexWidth();
    float getTexHeight();
    unsigned int getTextureId();

    TextureImageRegion(unsigned int textureId,
        float offsetX , float offsetY , 
        float width , float height,
        float texWidth , float texHeight){
        textureId_ = textureId;
        offsetX_ = offsetX;
        offsetY_ = offsetY;
        width_ = width;
        height_ = height;
        texWidth_ = texWidth;
        texHeight_ = texHeight; 
    } 
private:
    unsigned int textureId_;
    float offsetX_;
    float offsetY_;
    float width_;
    float height_;
    float texWidth_;
    float texHeight_;
};

//从asset目录中加载纹理图片资源
std::shared_ptr<TextureImage> BuildImageByAsset(std::string path);