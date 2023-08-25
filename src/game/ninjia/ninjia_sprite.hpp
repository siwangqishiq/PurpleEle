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
    WaitRun,
    Run,
    Dead
};

class NinjiaPlayer : public GameBaseObject {
public:
    NinjiaPlayer(NinjiaGame *game): GameBaseObject(game){
    };

    NinjaPlayerState playerState_;

    void init();

    void update();

    void render();

    void dispose();
private:
    Rect playerRect_;

    float baseLine_;
    float waitRunTime_ = 0.0f;

    float velocityX_ = 0.0f;
    float velocityY_ = 0.0f;

    float accleX_ = 0.0f;
    float accleY_ = 0.0f;

    float playerTextureWidth_ = 64.0f;
    float playerTextureHeight_ = 128.0f;

    const int RunImageCount = 6;
    const int FrameLimit = 4;
    int runIndex_;
    int runWaitFrame_;//控制帧动画速率

    std::shared_ptr<TextureImage> ninjiaIdleImage_;
    std::shared_ptr<TextureImage> ninjiaRunImage_;
};
