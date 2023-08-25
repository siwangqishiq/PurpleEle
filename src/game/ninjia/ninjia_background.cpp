#include "ninjia_game.hpp"
#include "ninjia_background.hpp"
#include "render/render_batch.hpp"
#include "render/ui/view.hpp"
#include "resource/asset_manager.hpp"
#include "render/sprite.hpp"
#include "render/texture.hpp"

void SkyBackground::init(){
    skyBackgroundImage_ = BuildImageByAsset("sprite/sky_bg.png");
}

void SkyBackground::render(){
    Rect screenRect;
    screenRect.left = 0.0f;
    screenRect.top = gameContext_->viewHeight_;
    screenRect.width = gameContext_->viewWidth_;
    screenRect.height = gameContext_->viewHeight_;

    Rect srcRect = skyBackgroundImage_->getRect();
    srcRect.width = srcRect.width / 2.0f;

    auto batch = gameContext_->renderEngine_->getSpriteBatch();
    batch->begin();
    batch->renderImage(skyBackgroundImage_ , srcRect , screenRect);
    batch->end();
}

void SkyBackground::dispose(){
    skyBackgroundImage_->dispose();
}

void Terrain::init(){
    terrainImage_ = BuildImageByAsset("sprite/bg_ground.png");
    forestImage_ = BuildImageByAsset("sprite/bg_tree.png");

    float ratio = static_cast<float>(gameContext_->viewWidth_) / static_cast<float>(terrainImage_->getWidth());
    terrainHeight_ = static_cast<float>(terrainImage_->getHeight()) * ratio;
    Logi("terrain" , "terrainHeight_ = %f" , terrainHeight_);

    forestBgHeight_ = static_cast<float>(forestImage_->getHeight())
        * static_cast<float>(gameContext_->viewWidth_) / static_cast<float>(forestImage_->getWidth());
    forestTop_ = terrainHeight_ + forestBgHeight_ - 4.0f;
}

void Terrain::render(){
    auto batch = gameContext_->renderEngine_->getSpriteBatch();

    Rect forestDstRect;
    forestDstRect.left = 0.0f;
    forestDstRect.top = forestTop_;
    forestDstRect.width = gameContext_->viewWidth_;
    forestDstRect.height = forestBgHeight_;

    batch->begin();
    auto forestRect = forestImage_->getRect();
    batch->renderImage(forestImage_ , forestRect, forestDstRect);
    batch->end();



    Rect terrainDstRect;
    terrainDstRect.left = 0.0f;
    terrainDstRect.top = terrainHeight_;
    terrainDstRect.width = gameContext_->viewWidth_;
    terrainDstRect.height = terrainHeight_;

    Rect srcRect = terrainImage_->getRect();

    batch->begin();
    batch->renderImage(terrainImage_ , srcRect , terrainDstRect);
    batch->end();
}

void Terrain::dispose(){
    terrainImage_->dispose();
    forestImage_->dispose();
}
