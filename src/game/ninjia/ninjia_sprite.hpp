#pragma once

#include "ninjia_game_base.hpp"

class TextureImage;

class Camera : public GameBaseObject{
public:
    Camera(NinjiaGame *game) : GameBaseObject(game){
    }

    void init();

    void update();
};


enum NinjaPlayerState{
    Falling,
    Run,
    Dead
};

class NinjiaPlayer : public GameBaseObject {
public:
    NinjiaPlayer(NinjiaGame *game): GameBaseObject(game){
    };

    NinjaPlayerState playerState_;

    void init();

    void render();

    void dispose();
private:
    Rect playerRect_;

    float playerTextureWidth_ = 64.0f;
    float playerTextureHeight_ = 128.0f;

    std::shared_ptr<TextureImage> ninjiaIdleImage_;
    std::shared_ptr<TextureImage> ninjiaRunImage_;
};
