#include <memory>
#include "audio_windows.hpp"
#include "log.hpp"

void DefaultAudioService::loadSound(std::string path , std::string name){
    Logi("MacAudioService" , "load sound %s" , path.c_str());
}

void DefaultAudioService::loadMusic(std::string path, std::string name){
    Logi("MacAudioService" , "load music %s" , path.c_str());
}

void DefaultAudioService::playMusic(std::string name){
    Logi("MacAudioService" , "play music %s" , name.c_str());
}

void DefaultAudioService::playSound(std::string name){
    Logi("MacAudioService" , "play sound %s" , name.c_str());
}

void DefaultAudioService::dispose(){
    Logi("MacAudioService" , "Dispose()");
}

std::shared_ptr<IAudioService> buildAudioService(){
    return std::make_shared<DefaultAudioService>();
}

