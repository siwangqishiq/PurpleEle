/**
 * texture  
 * 
 */

#pragma once

#include <memory>
#include <unordered_map>
#include "log.hpp"

struct TextureInfo{
    std::string name;//纹理名称
    unsigned int textureId = -1;//纹理ID
    int width;
    int height;
};

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

    std::shared_ptr<TextureInfo> acquireTexture(std::string textureFilePath);

    std::shared_ptr<TextureInfo> loadTexture(std::string textureFilePath);

    std::string allTextureInfos();
private:
    static std::shared_ptr<TextureManager> instance_;

    std::unordered_map<std::string , std::shared_ptr<TextureInfo>> textureBank_;
};

