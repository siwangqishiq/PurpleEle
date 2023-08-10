
#include "audio.hpp"
#include "log.hpp"
#include "resource/asset_manager.hpp"
#include <functional>

#ifndef MINIAUDIO
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#endif

struct AudioEntityUserData{
    ma_decoder *decoder;
    bool *isPlay;
    bool *isLoop;
};

struct AudioEntity{
    std::string name;
    std::unique_ptr<uint8_t[]> pData;
    ma_device device;
    ma_device_config deviceConfig;
    ma_device_data_proc dataCallback;

    AudioEntityUserData userData;

    ma_decoder decoder;
    bool isPlay = false;
    bool isLoop = false;

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

std::shared_ptr<AudioEntity> AudioManager::loadAudioEntity(std::string path , bool playLoop){
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
    entity->isLoop = playLoop;

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

    entity->dataCallback = [](ma_device* pDevice, void* pOutput, const void* pInput, 
        ma_uint32 frameCount){
        // std::cout << "audio play callback" << std::endl;
        AudioEntityUserData *userData = static_cast<AudioEntityUserData *>(pDevice->pUserData);
//        ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
        ma_decoder* pDecoder = userData->decoder;
        if (pDecoder == nullptr) {
            return;
        }

        if(*userData->isPlay){
            ma_uint64 readCount;
            ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, &readCount);
            if(readCount < frameCount){
                // std::cout << "audio play is ended" << std::endl;
                ma_decoder_seek_to_pcm_frame(pDecoder , 0);
                if(!(*userData->isLoop)){
                    *userData->isPlay = false;
                }
            }
        }
    };

    // ma_data_source_set_looping(&entity->decoder, MA_TRUE);
    
    entity->deviceConfig = ma_device_config_init(ma_device_type_playback);
    entity->deviceConfig.playback.format   = entity->decoder.outputFormat;
    entity->deviceConfig.playback.channels = entity->decoder.outputChannels;
    entity->deviceConfig.sampleRate        = entity->decoder.outputSampleRate;

    entity->userData.isPlay = &entity->isPlay;
    entity->userData.decoder = &(entity->decoder);
    entity->userData.isLoop = &(entity->isLoop);

    entity->deviceConfig.pUserData = &(entity->userData);
    entity->deviceConfig.dataCallback = entity->dataCallback;

    if(ma_device_init(nullptr, &(entity->deviceConfig), &(entity->device)) != MA_SUCCESS) {
        Logi("audio" , "loadAudioResource ma_device_init error");
        ma_decoder_uninit(&(entity->decoder));
        return nullptr;
    }

    entity->isPlay = false;
    return entity;
}

void AudioManager::playAudioEntity(std::shared_ptr<AudioEntity> entity){
    if(entity == nullptr){
        return;
    }

    // ma_device_uninit(&entity->device);
    // ma_device_init(nullptr, &(entity->deviceConfig), &(entity->device));

    entity->isPlay = true;
    if (ma_device_start(&(entity->device)) != MA_SUCCESS) {
        Logi("audio" , "Failed to start playback device");
    }
}

void AudioManager::stopAudioEntity(std::shared_ptr<AudioEntity> entity){
    if(entity == nullptr){
        return;
    }

    entity->isPlay = false;
    if (ma_device_stop(&(entity->device)) != MA_SUCCESS) {
        Logi("audio" , "Failed to stop playback device");
    }
}

bool AudioManager::isPlayAudioEntity(std::shared_ptr<AudioEntity> entity){
    if(entity == nullptr){
        return false;
    }

    return entity->isPlay;
}

void AudioManager::restartAudioEntity(std::shared_ptr<AudioEntity> entity){
    if(entity == nullptr){
        return;
    }

    ma_decoder_seek_to_pcm_frame(&entity->decoder , 0);
    playAudioEntity(entity);
}

void AudioManager::releaseAudioEntity(std::shared_ptr<AudioEntity> entity){
    if(entity == nullptr){
        return;
    }

    entity->isPlay = false;
    ma_device_stop(&(entity->device));

    ma_device_uninit(&(entity->device));
    ma_decoder_uninit(&(entity->decoder));

    // need this?
    // entity->pData.release();
}

 //载入音频资源
std::shared_ptr<AudioEntity> AudioManager::loadAudio(std::string path,
     std::string key , bool playloop){
    auto entity = loadAudioEntity(path , playloop);
    if(audioMap_[key] != nullptr){
        releaseAudioEntity(audioMap_[key]);
    }

    audioMap_[key] = entity;
    return entity;
}

//播放音频
void AudioManager::playAudio(std::string key){
    auto entity = audioMap_[key];
    playAudioEntity(entity);
}

//停止播放
void AudioManager::stopAudio(std::string key){
    auto entity = audioMap_[key];
    stopAudioEntity(entity);
}

//重新开始播放
void AudioManager::restartAudio(std::string key){
    auto entity = audioMap_[key];
    restartAudioEntity(entity);
}

//
void AudioManager::releaseAudio(std::string key){
    auto entity = audioMap_[key];
    releaseAudioEntity(entity);
    audioMap_.erase(key);
}

void AudioManager::init(){
    Logi("audio" , "audio manager init");
}

//释放载入的音频资源    
void AudioManager::dispose(){
    Logi("audio" , "audio manager dispose");

    for(auto &pair : audioMap_){
        if(pair.second != nullptr){
            releaseAudioEntity(pair.second);
        }
    }//end for each;

    audioMap_.clear();
}

