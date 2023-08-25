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

    playerRect_.left = 100.0f;
    playerRect_.width = 64.0f;
    playerRect_.height = 128.0f;
    playerRect_.top = gameContext_->terrain_->terrainHeight_ + playerRect_.height;
}

void NinjiaPlayer::render(){
    if(playerState_ == Falling){
        auto batch = gameContext_->renderEngine_->getSpriteBatch();
        batch->begin();
        auto srcRect = ninjiaIdleImage_->getRect();
        batch->renderImage(ninjiaIdleImage_ , srcRect, playerRect_);
        batch->end();
    }
}

void NinjiaPlayer::dispose(){
    ninjiaIdleImage_->dispose();
}

