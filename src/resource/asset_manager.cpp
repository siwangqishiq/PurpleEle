
#include "asset_manager.hpp"
#include "log.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"


std::shared_ptr<AssetManager> AssetManager::getInstance(){
    static std::shared_ptr<AssetManager> instance_;
    if(instance_ == nullptr){
        #ifdef __ANDROID__
        instance_ = std::make_shared<AndroidAssetManager>();
        #else
        instance_ = std::make_shared<AssetManager>();
        #endif
    }
    return instance_;
}

std::wstring AssetManager::readTextFile(std::string path){
    std::string filePath = assetRootDir() + path;
    Logi("asset" , "read file path %s" , filePath.c_str());
    return readFileAsWstring(filePath.c_str());
}

std::unique_ptr<uint8_t> AssetManager::readTextureFile(std::string path ,
        TextureFileConfig &fileConfig){
    std::string filePath = assetRootDir() + path;
    Logi("asset" , "read file path %s" , filePath.c_str());

    uint8_t *data = stbi_load(filePath.c_str() ,
                        &fileConfig.width , 
                        &fileConfig.height , &fileConfig.channel , 0);
    fileConfig.dataSize = fileConfig.width * fileConfig.height * fileConfig.channel;
    Logi("asset" , "read file size : %d , %d , %d" , fileConfig.width , fileConfig.height , fileConfig.channel);
    return std::unique_ptr<uint8_t>(data);
}


#ifdef __ANDROID__

#include <android/bitmap.h>
#include <android/imagedecoder.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

std::wstring AndroidAssetManager::readTextFile(std::string path){
    std::string filePath = path;
    AAsset *file = AAssetManager_open(AndroidAssetManagerInstance , filePath.c_str(), AASSET_MODE_BUFFER);
    if(file == nullptr){
        Logi("asset","open asset file : %s failed" , filePath.c_str());
        return L"";
    }

    Logi("asset" , "get asset file");
    size_t fileSize = AAsset_getLength(file);

    auto fileContentBuf = std::unique_ptr<char []>(new char[fileSize + 1]);
    // char *fileContentBuf = new char[fileSize + 1];
    AAsset_read(file , fileContentBuf.get() , fileSize);
    fileContentBuf[fileSize] = '\0';
    AAsset_close(file);
    
    LOGI("file content = %s" , fileContentBuf.get());
    std::wstring contentStr = toWideString(std::string(fileContentBuf.get()));
    // delete[] fileContentBuf;

    return contentStr;
}

std::unique_ptr<uint8_t> AndroidAssetManager::readTextureFile(std::string path ,TextureFileConfig &fileConfig){
    Logi("asset" , "android version %d" , android_get_device_api_level());

    if(android_get_device_api_level() >= 33){
        return readTextureFileByImageDecoder(path , fileConfig);
    }else{
        return readTextureFileByStbi(path , fileConfig);
    }
}

std::unique_ptr<uint8_t> AndroidAssetManager::readTextureFileByImageDecoder(std::string path ,TextureFileConfig &fileConfig){
#if __ANDROID_MIN_SDK_VERSION__ >= 23
    Logi("asset" , "decode image using image decoder");
    const char *filePath = path.c_str();
    Logi("asset" , "file path : %s" , filePath);

    AAsset* asset = AAssetManager_open(AndroidAssetManagerInstance, filePath, AASSET_MODE_STREAMING);
    AImageDecoder *decoder;
    int result = AImageDecoder_createFromAAsset(asset, &decoder);
    if (result != ANDROID_IMAGE_DECODER_SUCCESS) {
        AAsset_close(asset);
        Logi("asset" , "image decoder create failed");
        return nullptr;
    }

    Logi("asset" , "image decoder create success");
    const AImageDecoderHeaderInfo* info = AImageDecoder_getHeaderInfo(decoder);
    auto width = AImageDecoderHeaderInfo_getWidth(info);
    auto height = AImageDecoderHeaderInfo_getHeight(info);
    auto stride = AImageDecoder_getMinimumStride(decoder);
    AndroidBitmapFormat format = (AndroidBitmapFormat) AImageDecoderHeaderInfo_getAndroidBitmapFormat(info);
    Logi("asset" , "width %d , height %d , format %d ,stride %d" , width , height , format , stride);

    const size_t dataLength = height * stride;
    std::unique_ptr<uint8_t> pixels = std::unique_ptr<uint8_t>(new uint8_t[dataLength]);
    result = AImageDecoder_decodeImage(decoder, pixels.get(), stride, dataLength);

    fileConfig.width = width;
    fileConfig.height = height;
    fileConfig.dataSize = dataLength;
    if(format == ANDROID_BITMAP_FORMAT_RGBA_8888
        || format == ANDROID_BITMAP_FORMAT_RGBA_4444
        || format == ANDROID_BITMAP_FORMAT_RGBA_F16){
        fileConfig.channel = TEXTURE_FILE_CHANNEL_RGBA;
    }else if(format == ANDROID_BITMAP_FORMAT_RGB_565){
        fileConfig.channel = TEXTURE_FILE_CHANNEL_RGB;
    }

    if (result != ANDROID_IMAGE_DECODER_SUCCESS) {
        AImageDecoder_delete(decoder);
        AAsset_close(asset);
        Logi("asset" , "decode image file error");
        return nullptr;
    }

    AImageDecoder_delete(decoder);
    AAsset_close(asset);
    return pixels;
#else
    int minSdk = __ANDROID_API__ ;
    Logi("asset" , "decode image using image decoder min version %d" , __ANDROID_API__ );
    return nullptr;
#endif
}

std::unique_ptr<uint8_t> AndroidAssetManager::readTextureFileByStbi(std::string path ,TextureFileConfig &fileConfig){
    Logi("asset" , "decode image using stbi library");
    auto assetFile  = AAssetManager_open(AndroidAssetManagerInstance , path.c_str() , AASSET_MODE_STREAMING);
    size_t fileSize = AAsset_getLength(assetFile);
    auto contentBuf = std::unique_ptr<char []>(new char[fileSize]);
    AAsset_read(assetFile, contentBuf.get() , fileSize);
    AAsset_close(assetFile);

    uint8_t *data = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(contentBuf.get()), fileSize , &fileConfig.width ,
                                          &fileConfig.height , &fileConfig.channel, 0);
    fileConfig.dataSize = fileConfig.width * fileConfig.height * fileConfig.channel;
    return std::unique_ptr<uint8_t>(data);
}
#endif

