#pragma once

#include <memory>
#include <string>
#include <vector>
#include "common.hpp"

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

    Rect getRect();

    virtual int getWidth();
    virtual int getHeight();
    virtual unsigned int getTextureId();

    virtual void dispose();

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

//动画
class TextureImageAnimation{
private:
    std::vector<std::shared_ptr<TextureImageRegion>> imageRegionList_;
    int currentFrame_ = 0;
    long duration_ = 200;
public:
    TextureImageAnimation();

    ~TextureImageAnimation();
};

//从asset目录中加载纹理图片资源
std::shared_ptr<TextureImage> BuildImageByAsset(std::string path);

//加载一张空的纹理 纹理内容
std::shared_ptr<TextureImage> BuildEmptyImage(int width , int height , int format);