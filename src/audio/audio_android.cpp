#include <memory>
#include "audio_android.hpp"
#include "log.hpp"

void DefaultAudioService::loadSound(std::string path , std::string name){
    Logi("AndroidAudioService" , "load sound %s" , path.c_str());
}

void DefaultAudioService::loadMusic(std::string path, std::string name){
    Logi("AndroidAudioService" , "load music %s" , path.c_str());
}

void DefaultAudioService::playMusic(std::string name){
    Logi("AndroidAudioService" , "play music %s" , name.c_str());
}

void DefaultAudioService::playSound(std::string name){
    Logi("AndroidAudioService" , "play sound %s" , name.c_str());
}

void DefaultAudioService::dispose(){
    Logi("AndroidAudioService" , "Dispose()");
}

std::shared_ptr<IAudioService> buildAudioService(){
    return std::make_shared<DefaultAudioService>();
}

