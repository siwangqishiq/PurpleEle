
#include "audio.hpp"
#include "log.hpp"
#include "resource/asset_manager.hpp"
#include <functional>

#ifndef MINIAUDIO
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#endif

struct AudioEntity{
    std::string name;
    std::unique_ptr<uint8_t[]> pData;
    ma_device device;
    ma_device_config deviceConfig;
    ma_decoder decoder;

    ma_device_data_proc dataCallback;
    // std::function<void(
    //     ma_device* pDevice, 
    //     void* pOutput, 
    //     const void* pInput, 
    //     ma_uint32 frameCount)> dataCallback;
    
    // void dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
    // }
};

std::shared_ptr<AudioManager> AudioManager::getInstance(){
    static std::shared_ptr<AudioManager> instance_;
    if(instance_ == nullptr){
        instance_ = std::make_shared<AudioManager>();
    }
    return instance_;
}

void AudioManager::init(){
    Logi("audio" , "audio manager init");
}

std::shared_ptr<AudioEntity> AudioManager::loadAudioEntity(std::string path){
    // std::vector<char> dataVec;
    // AssetManager::getInstance()->readBinaryFile(path , dataVec);
    // Logi("audio" , "loadAudioResource file size: %d" , dataVec.size());
    
    int fileSize = 0;
    auto fileData = AssetManager::getInstance()->readFileAsBin(path , fileSize);
    Logi("audio" , "loadAudioResource file size: %d" , fileSize);
    if(fileSize <= 0){
        Logi("audio" , "loadAudioResource read file %s failed" , path.c_str());
    }
    
    std::shared_ptr<AudioEntity> entity = std::make_shared<AudioEntity>();
    entity->pData.reset(fileData.release());

    if(ma_decoder_init_memory(entity->pData.get() , 
            fileSize , nullptr , &entity->decoder) != MA_SUCCESS){
        Logi("audio" , "loadAudioResource %s create decoder error" , path.c_str());
        return nullptr;
    }

    Logi("audio" , "%s info channel: %d  sampleRate: %d  outputFormat : %d" , 
        path.c_str() , 
        entity->decoder.outputChannels,
        entity->decoder.outputSampleRate,
        entity->decoder.outputFormat);

    entity->dataCallback = [](ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount){
        std::cout << "audio play callback" << std::endl;
        ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
        if (pDecoder == NULL) {
            return;
        }
        ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, nullptr);
    };

    entity->deviceConfig = ma_device_config_init(ma_device_type_playback);
    entity->deviceConfig.playback.format   = entity->decoder.outputFormat;
    entity->deviceConfig.playback.channels = entity->decoder.outputChannels;
    entity->deviceConfig.sampleRate        = entity->decoder.outputSampleRate;
    entity->deviceConfig.pUserData         = &(entity->decoder);
    entity->deviceConfig.dataCallback      = entity->dataCallback;

    if(ma_device_init(NULL, &(entity->deviceConfig), &(entity->device)) != MA_SUCCESS) {
        Logi("audio" , "loadAudioResource ma_device_init error");
        ma_decoder_uninit(&(entity->decoder));
        return nullptr;
    }

    if (ma_device_start(&(entity->device)) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&(entity->device));
        ma_decoder_uninit(&(entity->decoder));
        return nullptr;
    }
    return entity;
}

//载入音效
void AudioManager::loadAudioResource(std::string path , std::string key){

}

//释放音效
void AudioManager::releaseAudioResource(std::string key){

}

 //播放音乐
void AudioManager::playMusic(std::string key , bool loop){

}

//暂停音乐播放
void AudioManager::pauseMusic(std::string key){

}

//重新开始音乐播放
void AudioManager::resetPlayMusic(std::string key){

}

//播放音效
void AudioManager::playSound(std::string resKey){

}
    
void AudioManager::dispose(){
    Logi("audio" , "audio manager dispose");
}

