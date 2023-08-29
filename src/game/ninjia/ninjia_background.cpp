#include "ninjia_game.hpp"
#include "ninjia_background.hpp"
#include "render/render_batch.hpp"
#include "render/ui/view.hpp"
#include "resource/asset_manager.hpp"
#include "render/sprite.hpp"
#include "render/texture.hpp"

void SkyBackground::init(){
    skyBackgroundImage_ = BuildImageByAsset("sprite/sky_bg.png");
    offset_ = 0.0f;
}

void SkyBackground::update(){
    //do nothing
}

void SkyBackground::renderByCamera(Camera &cam){
    float skyBgRealOffset = SCROLL_SPEED_RATIO * gameContext_->player_->getPlayerRect().left;
    int num = static_cast<int>(skyBgRealOffset) / static_cast<int>(gameContext_->viewWidth_);
    offset_ = skyBgRealOffset - num * gameContext_->viewWidth_;

    Rect dstRect;
    dstRect.top = gameContext_->viewHeight_;
    dstRect.width = 2.0f * gameContext_->viewWidth_;
    dstRect.height = gameContext_->viewHeight_;
    dstRect.left = -offset_;

    Rect srcRect = skyBackgroundImage_->getRect();

    auto batch = gameContext_->renderEngine_->getSpriteBatch();
    batch->begin();
    batch->renderImage(skyBackgroundImage_ , srcRect , dstRect);
    batch->end();
}

void SkyBackground::render(){
    Rect screenRect;
    screenRect.left = 0.0f;
    screenRect.top = gameContext_->viewHeight_;
    screenRect.width = 2.0f * gameContext_->viewWidth_;
    screenRect.height = gameContext_->viewHeight_;

    Rect srcRect = skyBackgroundImage_->getRect();
    // srcRect.width = srcRect.width / 2.0f;

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

    terrainHeight_ = gameContext_->viewHeight_ / 5.0f;

    // float ratio = static_cast<float>(gameContext_->viewWidth_) / static_cast<float>(terrainImage_->getWidth());
    // terrainHeight_ = static_cast<float>(terrainImage_->getHeight()) * ratio;
    // Logi("terrain" , "terrainHeight_ = %f" , terrainHeight_);

    float ratio = terrainHeight_ / static_cast<float>(terrainImage_->getHeight());

    // forestBgHeight_ = static_cast<float>(forestImage_->getHeight())
    //     * static_cast<float>(gameContext_->viewWidth_) / static_cast<float>(forestImage_->getWidth());

    forestBgHeight_ = ratio * static_cast<float>(forestImage_->getHeight());
    forestTop_ = terrainHeight_ + forestBgHeight_ - 4.0f;

    firstTileX_ = 0.0f;
    secondTileX_ = gameContext_->viewWidth_;

    lastPlayerPosX_ = 0.0f;

    storesImage_ = BuildImageByAsset("sprite/stones.png");

    storeImageList_.push_back(storesImage_->createImageRegion(0 , storesImage_->getHeight() , 53.0f , 78.0f));
    storeImageList_.push_back(storesImage_->createImageRegion(55.0f , storesImage_->getHeight() , 62.0f , 50.0f));
    storeImageList_.push_back(storesImage_->createImageRegion(120.0f , storesImage_->getHeight() , 43.0f , 47.0f));
    storeImageList_.push_back(storesImage_->createImageRegion(163.0f , storesImage_->getHeight() , 46.0f , 42.0f));
    storeImageList_.push_back(storesImage_->createImageRegion(214.0f , storesImage_->getHeight() , 42.0f , 48.0f));
    storeImageList_.push_back(storesImage_->createImageRegion(260.0f , storesImage_->getHeight() , 50.0f , 90.0f));
}

void Terrain::update(){
    float deltaX = gameContext_->player_->getPlayerRect().left - lastPlayerPosX_;

    firstTileX_ += -deltaX;
    if(firstTileX_ < -gameContext_->camera_->cameraViewWidth_){
        firstTileX_ = 0.0f;
    }

    secondTileX_ = firstTileX_ + gameContext_->camera_->cameraViewWidth_;

    lastPlayerPosX_ = gameContext_->player_->getPlayerRect().left;
}

void Terrain::renderByCamera(Camera &cam){
    auto batch = gameContext_->renderEngine_->getSpriteBatch();

    Rect firstForestDstRect;
    firstForestDstRect.left = firstTileX_;
    firstForestDstRect.top = forestTop_;
    firstForestDstRect.width = gameContext_->viewWidth_;
    firstForestDstRect.height = forestBgHeight_;

    Rect secondForestDstRect;
    secondForestDstRect.left = secondTileX_;
    secondForestDstRect.top = forestTop_;
    secondForestDstRect.width = gameContext_->viewWidth_;
    secondForestDstRect.height = forestBgHeight_;

    batch->begin();
    auto forestRect = forestImage_->getRect();
    // forestRect.width = forestRect.height * ((forestDstRect.width / forestDstRect.height));
    batch->renderImage(forestImage_ , forestRect, firstForestDstRect);
    batch->renderImage(forestImage_ , forestRect, secondForestDstRect);
    batch->end();

    Rect firstTerrainDstRect;
    firstTerrainDstRect.left = firstTileX_;
    firstTerrainDstRect.top = terrainHeight_;
    firstTerrainDstRect.width = gameContext_->viewWidth_;
    firstTerrainDstRect.height = terrainHeight_;

    Rect secondTerrainDstRect;
    secondTerrainDstRect.left = secondTileX_;
    secondTerrainDstRect.top = terrainHeight_;
    secondTerrainDstRect.width = gameContext_->viewWidth_;
    secondTerrainDstRect.height = terrainHeight_;
    // srcRect.width = srcRect.height *(terrainDstRect.width / terrainDstRect.height);
    batch->begin();
    Rect srcRect = terrainImage_->getRect();
    batch->renderImage(terrainImage_ , srcRect , firstTerrainDstRect);
    batch->renderImage(terrainImage_ , srcRect , secondTerrainDstRect);
    batch->end();

    //  std::cout << "forestBgHeight_ : " << forestBgHeight_
    //      << "  firstForestDstRect.width : " << firstForestDstRect.width
    //      << " firstForestDstRect.left " << firstForestDstRect.left
    //      << "  firstTileX_ " << firstTileX_ <<  std::endl;


    //for test
    batch->begin();
    Rect rect;
    rect.left = 0.0f;
    rect.top = terrainHeight_;
    rect.width = 128.0f;
    rect.height = terrainHeight_;

    for(auto img : storeImageList_){
        batch->renderRegionImage(*img , rect);
        rect.left += rect.width + 20.0f;
    }
    batch->end();
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
    forestRect.width = forestRect.height * ((forestDstRect.width / forestDstRect.height));
    batch->renderImage(forestImage_ , forestRect, forestDstRect);
    batch->end();

    Rect terrainDstRect;
    terrainDstRect.left = 0.0f;
    terrainDstRect.top = terrainHeight_;
    terrainDstRect.width = gameContext_->viewWidth_;
    terrainDstRect.height = terrainHeight_;

    Rect srcRect = terrainImage_->getRect();
    srcRect.width = srcRect.height *(terrainDstRect.width / terrainDstRect.height);
    batch->begin();
    batch->renderImage(terrainImage_ , srcRect , terrainDstRect);
    batch->end();

    // std::cout << "terrainHeight_ : " << terrainHeight_ << std::endl;
}

void Terrain::dispose(){
    terrainImage_->dispose();
    forestImage_->dispose();
}
