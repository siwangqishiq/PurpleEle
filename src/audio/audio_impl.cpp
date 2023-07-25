#include <memory>
#include "audio_impl.hpp"
#include "log.hpp"

void DefaultAudioService::loadSound(std::string path , std::string name){
    Logi("DefaultAudioService" , "load sound %s" , path.c_str());
}

void DefaultAudioService::loadMusic(std::string path, std::string name){
    Logi("DefaultAudioService" , "load music %s" , path.c_str());
}

void DefaultAudioService::playMusic(std::string name){
    Logi("DefaultAudioService" , "play music %s" , name.c_str());
}

void DefaultAudioService::playSound(std::string name){
    Logi("DefaultAudioService" , "play sound %s" , name.c_str());
}

void DefaultAudioService::dispose(){
    Logi("DefaultAudioService" , "Dispose()");
}

std::shared_ptr<IAudioService> buildAudioService(){
    return std::make_shared<DefaultAudioService>();
}

