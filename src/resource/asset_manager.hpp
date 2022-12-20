
/**
 * @brief  
 * resource access 
 * 
 */
#pragma once

#include <string>
#include <memory>
#include "log.hpp"
#include <fstream>
#include <streambuf>
#include <codecvt>

const int TEXTURE_FILE_CHANNEL_UNKNOW = -1;
const int TEXTURE_FILE_CHANNEL_RGB = 3;
const int TEXTURE_FILE_CHANNEL_ARGB = 4;
const int TEXTURE_FILE_CHANNEL_RGBA = 5;


struct TextureFileConfig{
    int width = 0;
    int height = 0;
    long dataSize = 0;
    int channel = TEXTURE_FILE_CHANNEL_UNKNOW;
};

class AndroidAssetManager;

class AssetManager{
public:
    static std::shared_ptr<AssetManager> getInstance();

    static std::shared_ptr<AssetManager> instance_;

    // read text file
    virtual std::wstring readTextFile(std::string path);

    // read png file 
    virtual std::unique_ptr<uint8_t> readTextureFile(std::string path ,
                TextureFileConfig &fileConfig);
    
    inline std::string readTextFileAsString(std::string path){
        return toByteString(readTextFile(path));
    }

    AssetManager(){
        Logi("asset_manager" , "asset manager construct");
    }

    ~AssetManager(){
        Logi("asset_manager" , "~asset manager deconstruct");
    }

protected:
    std::string assetRootDir() {
        return "../assets/";
    }

protected:
     inline std::wstring toWideString(const std::string& input){
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(input);
    }

    inline std::string toByteString(const std::wstring& input){
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(input);
    }

    inline std::wstring readFileAsWstring(const char *path){
        std::ifstream file(path);
        std::string str((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
        return toWideString(str);
    }
};

#ifdef __ANDROID__
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>
#include <android/imagedecoder.h>
extern AAssetManager *AndroidAssetManagerInstance;

/**
 * @brief
 * for android asset resouce access
 *
 */
class AndroidAssetManager : public AssetManager{
public:
    virtual std::wstring readTextFile(std::string path);

    virtual std::unique_ptr<uint8_t> readTextureFile(std::string path ,
                                                      TextureFileConfig &fileConfig);

private:
    std::unique_ptr<uint8_t> readTextureFileByImageDecoder(std::string path ,TextureFileConfig &fileConfig);

    std::unique_ptr<uint8_t> readTextureFileByStbi(std::string path ,TextureFileConfig &fileConfig);
};

#endif

