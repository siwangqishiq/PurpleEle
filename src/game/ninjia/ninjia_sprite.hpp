#pragma once

#include "ninjia_game_base.hpp"

class TextureImage;

class Camera : public GameBaseObject{
public:
    Camera(NinjiaGame *game) : GameBaseObject(game){
    }

    void init();

    void update();

    float camerLeft_;
    float cameraTop_;
    float cameraViewWidth_;
    float cameraViewHeight_;
};


enum NinjaPlayerState{
    Falling,
    WaitRun,
    Run,
    Dead
};


// const float NINJA_MAX_VELOCITY = 12.0f;
const float GRAVITY = -3.0f;
const float ACCELERATION_X = 0.5f;

class NinjiaPlayer : public GameBaseObject {
public:
    NinjiaPlayer(NinjiaGame *game): GameBaseObject(game){
    };

    NinjaPlayerState playerState_;

    void init();

    void update();

    void render();

    //render this player from camera view
    void renderByCamera(Camera &camera);

    bool jump();

    void hitted();

    void dispose();

    Rect getPlayerRect(){
        return playerRect_;
    }

    int getLifeCount(){
        return lifeCount;
    }
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
    int frameLimit = 4;
    int runIndex_;
    int runWaitFrame_ = 0;//控制帧动画速率

    std::shared_ptr<TextureImage> ninjiaIdleImage_;
    std::shared_ptr<TextureImage> ninjiaRunImage_;

    bool isJumping = false;

    float jumpMaxVelocity_;
    float ninjiaMaxVelocity_;

    int lifeCount = 3;
};
