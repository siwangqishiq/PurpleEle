#include "ninjia_game.hpp"
#include "ninjia_sprite.hpp"
#include "render/render_batch.hpp"
#include "render/ui/view.hpp"
#include "resource/asset_manager.hpp"
#include "render/sprite.hpp"
#include "render/texture.hpp"
#include "audio/audio.hpp"

void Camera::init(){
    cameraViewWidth_ = gameContext_->viewWidth_;
    cameraViewHeight_ = gameContext_->viewHeight_;

    cameraTop_ = cameraViewHeight_;
    camerLeft_ = 0.0f;
}

void Camera::update(){
    float leftLimit = cameraViewWidth_ / 2.5f;
    auto playerRect = gameContext_->player_->getPlayerRect();
    float playerX = playerRect.left;

    //update the camera position
    if(playerX <= leftLimit){
        camerLeft_ = 0.0f;
    }else{
        camerLeft_ = playerX - leftLimit;
    }

}

void NinjiaPlayer::init(){
    playerState_ = NinjaPlayerState::Falling;

    runWaitFrame_ = 0;

    ninjiaIdleImage_ = BuildImageByAsset("sprite/ninjia_idle.png");
    ninjiaRunImage_ = BuildImageByAsset("sprite/ninjia_run.png");

    playerRect_.left = 100.0f;
    playerRect_.height = gameContext_->terrain_->terrainHeight_;
    float ratio = static_cast<float>(ninjiaIdleImage_->getWidth()) / ninjiaIdleImage_->getHeight();
    playerRect_.width = playerRect_.height * ratio;

    // playerRect_.top = gameContext_->terrain_->terrainHeight_ + playerRect_.height;
    playerRect_.top = gameContext_->viewHeight_;

    baseLine_ = gameContext_->terrain_->terrainHeight_ + playerRect_.height;

    accleY_ = GRAVITY;
    
    jumpMaxVelocity_ = glm::sqrt(2.0f * glm::abs(GRAVITY) * (gameContext_->viewHeight_ / 3.0f));

    // ninjiaMaxVelocity_ = jumpMaxVelocity_;
    ninjiaMaxVelocity_ = playerRect_.width / 7.0f;
}

void NinjiaPlayer::update(){
    float deltaTime = 1.0f;
    velocityX_ += accleX_ * deltaTime;
    velocityY_ += accleY_ * deltaTime;

    if(velocityX_ > ninjiaMaxVelocity_){
        velocityX_ = ninjiaMaxVelocity_;
    }

    frameLimit = velocityX_ <= 10.0f?4:2;

    playerRect_.left += velocityX_ * deltaTime;
    playerRect_.top += velocityY_ * deltaTime;

    // std::cout << "  vy = " << velocityY_ << " pos " << playerRect_.top << std::endl;

    //logic update
    switch (playerState_){
    case Falling:
        if(playerRect_.top <= gameContext_->terrain_->terrainHeight_ + playerRect_.height){
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
            
            accleX_ = ACCELERATION_X;
            accleY_ = 0.0f;

            velocityY_ = 0.0f;

            runIndex_ = 0;
        }
        break;
    case Run:
        // std::cout << "runIndex_ = " << runIndex_<<std::endl;
        if(!isJumping){
            runWaitFrame_++;
            // std::cout << "runWaitFrame_ = " << runWaitFrame_<<std::endl;
            if(runWaitFrame_ >= frameLimit){
                // std::cout << "runIndex_++ = " << runIndex_<<std::endl;
                runIndex_ = (runIndex_+ 1) % RunImageCount;
                runWaitFrame_ = 0;
            }
        }else{
            runIndex_ = 0;
        }

        if(playerRect_.top <= gameContext_->terrain_->terrainHeight_ + playerRect_.height){
            // std::cout <<playerRect_.getBottom() << "  baseline " << baseLine_
            //     << " playerRect_.getBottom() <= baseLine_ " << std::endl;
            playerRect_.top = gameContext_->terrain_->terrainHeight_ + playerRect_.height;
            accleY_ = 0.0f;
            velocityY_ = 0.0f;
            isJumping = false;
        }
        break;
    default:
        break;
    }//end switch

    if(gameContext_->terrain_->collisionDetect()){
        this->hitted();
    }

    int addScore = gameContext_->terrain_->scoreUpdate();
    if(addScore > 0){
        gameContext_->playerGetScore(addScore);
    }
}

void NinjiaPlayer::renderByCamera(Camera &camera){
    Rect dstRect = playerRect_;
    // float limitX = gameContext_->viewWidth_ / 2.0f;
    // if(dstRect.getRight() >= limitX){
    //     dstRect.left = limitX - dstRect.width;
    // }

    dstRect.left = dstRect.left - camera.camerLeft_;

    if(playerState_ == Falling){
        auto batch = gameContext_->renderEngine_->getSpriteBatch();
        batch->begin();
        auto srcRect = ninjiaIdleImage_->getRect();
        batch->renderImage(ninjiaIdleImage_ , srcRect, dstRect);
        batch->end();
    } else if(playerState_ == WaitRun){
        auto batch = gameContext_->renderEngine_->getSpriteBatch();
        batch->begin();
        auto srcRect = ninjiaIdleImage_->getRect();
        batch->renderImage(ninjiaIdleImage_ , srcRect, dstRect);
        batch->end();
    }else if(playerState_ == Run){
        auto batch = gameContext_->renderEngine_->getSpriteBatch();
        batch->begin();
        auto srcRect = ninjiaRunImage_->getRect();

        float runImageWidth = static_cast<float>(ninjiaRunImage_->getWidth()) / RunImageCount;
        srcRect.left = runImageWidth * runIndex_;
        srcRect.width = runImageWidth;
        batch->renderImage(ninjiaRunImage_ , srcRect, dstRect);
        batch->end();

        //for debug
//        auto shapeBatch = gameContext_->renderEngine_->getShapeBatch();
//        shapeBatch->begin();
//        Paint paint;
//        paint.color = glm::vec4 (1.0f , 0.0f , 0.0f , 0.3f);
//        shapeBatch->renderRect(dstRect , paint);
//        shapeBatch->end();
    }
}

bool NinjiaPlayer::jump(){
    if(isJumping){
        return false;
    }

    //I = m * v;
    accleY_ = GRAVITY;
    velocityY_ = jumpMaxVelocity_;
    // playerRect_.top += velocityY_;

    // Logi("ninjia" , "ninja jump!");
    isJumping = true;
    return true;
}

void NinjiaPlayer::hitted() {
    AudioManager::getInstance()->playAudio(gameContext_->AUDIO_STONE_HIT);
    velocityY_ = 0.0f;
    velocityX_ = 0.0f;

    lifeCount--;
    if(lifeCount <= 0 ){
        gameContext_->gameState_ = End;
        AudioManager::getInstance()->stopAudio(gameContext_->AUDIO_BGM);
        AudioManager::getInstance()->playAudio(gameContext_->AUDIO_GAME_OVER);
    }
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

