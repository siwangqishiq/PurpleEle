/**
 * texture  
 * 
 */

#pragma once

#include <memory>
#include <unordered_map>
#include "log.hpp"
#include <vector>
#include "glheader.hpp"

enum TextureType{
    TEXTURE_2D,
    TEXTURE_2D_ARRAY
};

struct TextureInfo{
    std::string name;//纹理名称
    unsigned int textureId = -1;//纹理ID
    int width;
    int height;
    TextureType type = TEXTURE_2D;
    int depth;
};

static GLint convertChanelToInternalFormat(int channelFormat);

//纹理管理
class TextureManager{
public:
    static std::shared_ptr<TextureManager> getInstance();

    const std::string TAG = "TextureManager";

    TextureManager(){
        Logi(TAG , "TextureManager construct");
    }

    ~TextureManager(){
        Logi(TAG , "TextureManager deconstruct");
        clear();
    }
    
    void clear();

    void freeTexture(TextureInfo &info);

    std::shared_ptr<TextureInfo> acquireTexture(std::string textureFilePath , bool needFlip = false);

    std::shared_ptr<TextureInfo> loadTexture(std::string textureFilePath ,bool needFlip = false);

    std::shared_ptr<TextureInfo> loadTextureArray(std::vector<std::string> &textureFiles , bool needFlip = false);

    std::string allTextureInfos();
private:
    static std::shared_ptr<TextureManager> instance_;

    std::unordered_map<std::string , std::shared_ptr<TextureInfo>> textureBank_;

    std::unique_ptr<uint8_t> readTextureFile(std::string &path ,bool needFlip
        , int &format , int &width , int &height);
};

