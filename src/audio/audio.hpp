
#include <string>
#include <memory>

class IAudioService{
public:
    //载入音效
    virtual void loadSound(std::string path , std::string name) = 0;

    //载入音乐
    virtual void loadMusic(std::string path , std::string name) = 0;

    //播放音乐
    virtual void playMusic(std::string name) = 0;

    //播放音效
    virtual void playSound(std::string name) = 0;

    //释放载入资源
    virtual void dispose() = 0;
};

extern std::shared_ptr<IAudioService> buildAudioService();

class AudioManager{
public:
    static std::shared_ptr<AudioManager> instance_;

    static std::shared_ptr<AudioManager> getInstance();

    void loadSound(std::string path , std::string name);

    void loadMusic(std::string path , std::string name);

    void playMusic(std::string name);

    void playSound(std::string name);
    
    void dispose();
private:
    std::shared_ptr<IAudioService> audioService_;
};


