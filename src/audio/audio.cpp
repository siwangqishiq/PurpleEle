
#include "audio.hpp"
#include "log.hpp"

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

