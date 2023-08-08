#pragma once

#include <string>
#include <memory>
#include <map>

class AudioEntity{

};


class AudioManager{
public:
    static std::shared_ptr<AudioManager> getInstance();

    void init();

    //载入音效
    virtual void loadAudioResource(std::string path , std::string key);

    //释放音效
    virtual void releaseAudioResource(std::string key);
    
    //播放音乐
    virtual void playMusic(std::string key , bool loop = true);

    //暂停音乐播放
    virtual void pauseMusic(std::string key);

    //重新开始音乐播放
    virtual void resetPlayMusic(std::string key);

    //播放音效
    virtual void playSound(std::string resKey);
    
    void dispose();

    std::shared_ptr<AudioEntity> loadAudioEntity(std::string path);
private:
    
};


