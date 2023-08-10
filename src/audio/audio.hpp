#pragma once

#include <string>
#include <memory>
#include <map>

struct AudioEntity;

class AudioManager{
public:
    static std::shared_ptr<AudioManager> getInstance();

    void init();

    //载入音频资源
    std::shared_ptr<AudioEntity> loadAudio(std::string path , std::string key ,bool playloop = false);

    //播放音频
    void playAudio(std::string key);

    //停止播放
    void stopAudio(std::string key);

    //重新开始播放
    void restartAudio(std::string key);

    //释放资源
    void releaseAudio(std::string key);
    
    void dispose();

    std::shared_ptr<AudioEntity> loadAudioEntity(std::string path , bool playLoop = false);

    void releaseAudioEntity(std::shared_ptr<AudioEntity> entity);

    void playAudioEntity(std::shared_ptr<AudioEntity> entity);

    void stopAudioEntity(std::shared_ptr<AudioEntity> entity);

    bool isPlayAudioEntity(std::shared_ptr<AudioEntity> entity);

    void restartAudioEntity(std::shared_ptr<AudioEntity> entity);

private:
    std::map<std::string , std::shared_ptr<AudioEntity>> audioMap_;
};


