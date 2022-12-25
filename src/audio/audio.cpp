
#include "audio.hpp"

std::shared_ptr<AudioManager> AudioManager::getInstance(){
    static std::shared_ptr<AudioManager> instance_;
    if(instance_ == nullptr){
        instance_ = std::make_shared<AudioManager>();
        instance_->audioService_ = buildAudioService();
    }
    return instance_;
}

void AudioManager::loadSound(std::string path , std::string name){
    audioService_->loadMusic(path , path);
}

void AudioManager::loadMusic(std::string path , std::string name){
    audioService_->loadMusic(path , path);
}

void AudioManager::playMusic(std::string name){
    audioService_->playMusic(name);
}

void AudioManager::playSound(std::string name){
    audioService_->playSound(name);
}

void AudioManager::dispose(){
    audioService_->dispose();
}

