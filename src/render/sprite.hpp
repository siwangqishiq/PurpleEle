#pragma once

#include <memory>
#include <string>

class Image{
public:
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual unsigned int getTextureId() = 0;
};

class TextureInfo;

class TextureImage : public Image{
public:
    TextureImage(std::shared_ptr<TextureInfo> textureInfo);

    virtual int getWidth();
    virtual int getHeight();
    virtual unsigned int getTextureId();

private:
    std::shared_ptr<TextureInfo> textureInfo_;
};

//从asset目录中加载纹理图片资源
std::shared_ptr<TextureImage> BuildImageByAsset(std::string path);