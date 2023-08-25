#include "ninjia_game.hpp"
#include "ninjia_sprite.hpp"
#include "render/render_batch.hpp"
#include "render/ui/view.hpp"
#include "resource/asset_manager.hpp"
#include "render/sprite.hpp"
#include "render/texture.hpp"

void Camera::init(){
    
}

void Camera::update(){

}

void NinjiaPlayer::init(){
    playerState_ = NinjaPlayerState::Falling;

    ninjiaIdleImage_ = BuildImageByAsset("sprite/ninjia_idle.png");
    ninjiaRunImage_ = BuildImageByAsset("sprite/ninjia_run.png");

    playerRect_.left = 100.0f;
    playerRect_.height = gameContext_->terrain_->terrainHeight_;
    float ratio = static_cast<float>(ninjiaIdleImage_->getWidth()) / ninjiaIdleImage_->getHeight();
    playerRect_.width = playerRect_.height * ratio;

    // playerRect_.top = gameContext_->terrain_->terrainHeight_ + playerRect_.height;
    playerRect_.top = gameContext_->viewHeight_;

    baseLine_ = gameContext_->terrain_->terrainHeight_ + playerRect_.height;

    accleY_ = -0.5f;
}

void NinjiaPlayer::update(){
    float deltaTime = 1.0f;
    velocityX_ += accleX_ * deltaTime;
    velocityY_ += accleY_ * deltaTime;

    if(velocityX_ > 20.0f){
        velocityX_ = 20.0f;
    }

    playerRect_.left += velocityX_ * deltaTime;
    playerRect_.top += velocityY_ * deltaTime;

    //logic update
    switch (playerState_){
    case Falling:
        if(playerRect_.getBottom() <= baseLine_){
            playerRect_.top = gameContext_->terrain_->terrainHeight_ + playerRect_.height;
            playerState_ = WaitRun;
            accleX_ = 0.0f;

            accleY_ = 0.0f;
            velocityY_ = 0.0f;
        }
        break;
    case WaitRun:
        waitRunTime_ += deltaTime;
        if(waitRunTime_ > 20.0f){
            playerState_ = Run;
            accleY_ = 0.0f;
            velocityY_ = 0.0f;
            accleX_ = 0.04f;

            runIndex_ = 0;
        }
        break;
    case Run:
        runWaitFrame_++;
        if(runWaitFrame_ >= FrameLimit){
            runIndex_ = (runIndex_+ 1) % RunImageCount;
            runWaitFrame_ = 0;
        }
        break;
    default:
        break;
    }//end switch
    
}

void NinjiaPlayer::render(){
    if(playerState_ == Falling){
        auto batch = gameContext_->renderEngine_->getSpriteBatch();
        batch->begin();
        auto srcRect = ninjiaIdleImage_->getRect();
        batch->renderImage(ninjiaIdleImage_ , srcRect, playerRect_);
        batch->end();
    } else if(playerState_ == WaitRun){
        auto batch = gameContext_->renderEngine_->getSpriteBatch();
        batch->begin();
        auto srcRect = ninjiaIdleImage_->getRect();
        batch->renderImage(ninjiaIdleImage_ , srcRect, playerRect_);
        batch->end();
    }else if(playerState_ == Run){
        auto batch = gameContext_->renderEngine_->getSpriteBatch();
        batch->begin();
        auto srcRect = ninjiaRunImage_->getRect();

        float runImageWidth = static_cast<float>(ninjiaRunImage_->getWidth()) / RunImageCount;
        srcRect.left = runImageWidth * runIndex_;
        srcRect.width = runImageWidth;
        batch->renderImage(ninjiaRunImage_ , srcRect, playerRect_);
        batch->end();
    }
}

void NinjiaPlayer::dispose(){
    ninjiaRunImage_->dispose();
    ninjiaIdleImage_->dispose();
}

