#pragma once

#include "audio.hpp"

class DefaultAudioService : public IAudioService{
    virtual void loadSound(std::string path, std::string name);

    virtual void loadMusic(std::string path, std::string name);

    virtual void playMusic(std::string name);

    virtual void playSound(std::string name);

    virtual void dispose();
};