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
//    float skyBgRealOffset = SCROLL_SPEED_RATIO * gameContext_->player_->getPlayerRect().left;
    float skyBgRealOffset = SCROLL_SPEED_RATIO * gameContext_->camera_->camerLeft_;

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

    // firstTileX_ = 0.0f;
    // secondTileX_ = gameContext_->viewWidth_;

    lastPlayerPosX_ = 0.0f;

    storesImage_ = BuildImageByAsset("sprite/stones.png");

    storeImageList_.push_back(storesImage_->createImageRegion(0 , storesImage_->getHeight() , 53.0f , 78.0f));
    storeImageList_.push_back(storesImage_->createImageRegion(55.0f , storesImage_->getHeight() , 62.0f , 50.0f));
    storeImageList_.push_back(storesImage_->createImageRegion(120.0f , storesImage_->getHeight() , 43.0f , 47.0f));
    storeImageList_.push_back(storesImage_->createImageRegion(163.0f , storesImage_->getHeight() , 46.0f , 42.0f));
    storeImageList_.push_back(storesImage_->createImageRegion(214.0f , storesImage_->getHeight() , 42.0f , 48.0f));
    storeImageList_.push_back(storesImage_->createImageRegion(260.0f , storesImage_->getHeight() , 50.0f , 90.0f));

    initTerrainMapData();
}

void Terrain::initTerrainMapData(){
    tileData_.clear();

    for(int i = 0 ; i < 200 ;i++){
        TerrainTile tile;
        tile.obstraces = std::make_shared<std::vector<Obstrace>>();
        if((i % 3 == 0 || i % 5 == 0) && i != 0){
            tile.type = TERRAIN_TILE_TYPE_NOFOREST;
        }else{
            tile.type = TERRAIN_TILE_TYPE_FOREST;
        }

        if(i > 1){
            addObstraceToTile(tile , i);//add obstraces
        }
        tileData_.push_back(tile);
    }//end for i
}

 void Terrain::addObstraceToTile(TerrainTile &tile , int index){
    float offset = index * gameContext_->viewWidth_;
    float obsWidth = terrainHeight_ * 0.8f;
    int obsCount = GenRandomInt(0 , 2);
    // std::cout << "debug obsCount = " << obsCount << std::endl;
    for(int i = 0 ; i < obsCount ; i++){
        Obstrace obs;

        float x = GenRandom(offset , offset + gameContext_->viewWidth_ - obsWidth);
        obs.type = GenRandomInt(0 , storeImageList_.size() - 1);
//        obs.type = 3;

        auto obsImage = storeImageList_[obs.type];
        float radio = obsImage->getWidth() / obsImage->getHeight();
        
        float obsHeight = obsWidth / radio;
        float y = obsHeight + terrainHeight_ - obsHeight / 2.0f;
        
        obs.position = glm::vec2(x , y);
        obs.size = glm::vec2(obsWidth , obsHeight);

        const float padding = 10.0f;
        obs.hitRect = Rect(x - padding , y - padding , obsWidth/2.0f , obsHeight - padding);
        tile.obstraces->push_back(obs);
    }//end for i
 }

void Terrain::update(Camera &cam){
    // float deltaX = gameContext_->player_->getPlayerRect().left - lastPlayerPosX_;

    // firstTileX_ += -deltaX;
    // if(firstTileX_ < -gameContext_->camera_->cameraViewWidth_){
    //     showForest = !showForest;
    //     firstTileX_ = 0.0f;
    // }

    // secondTileX_ = firstTileX_ + gameContext_->camera_->cameraViewWidth_;

    // lastPlayerPosX_ = gameContext_->player_->getPlayerRect().left;

    float cameraLeft = cam.camerLeft_;
    renderTileList_.clear();
    
    float tileWidth = gameContext_->viewWidth_;
    int renderIdx = static_cast<int>(cameraLeft / tileWidth);
    if(renderIdx < tileData_.size()){
        renderTileList_.push_back(renderIdx);
    }
    float cameraRight = cam.camerLeft_ + cam.cameraViewWidth_;

    while(renderIdx * tileWidth + tileWidth <= cameraRight){
        renderIdx++;
        if(renderIdx < tileData_.size()){
            renderTileList_.push_back(renderIdx);
        }else{
            break;
        }
    }//end while


}

bool Terrain::collisionDetect(){
    Rect playerRect = gameContext_->player_->getPlayerRect();
    for(int index : renderTileList_){
        auto obs = tileData_[index].obstraces;
        for(auto &ob : *obs){
            if(ob.state != Normal){
                continue;
            }

            if(CheckRectIntersect(playerRect , ob.hitRect)){
                ob.state = Hitted;
                return true;
            }
        }//end for each
    }//end for each index
    return false;
}

int Terrain::scoreUpdate(){
    int score = 0;
    Rect playerRect = gameContext_->player_->getPlayerRect();
    for(int index : renderTileList_){
        auto obs = tileData_[index].obstraces;
        for(auto &ob : *obs){
            if(ob.state != Normal || ob.addScore){
                continue;
            }
            
            if(ob.hitRect.getRight() < playerRect.left){
                score += 20;
                ob.addScore = true;
            }
        }//end for each
    }//end for each index
    return score;
}

void Terrain::renderByCamera(Camera &cam){
    auto batch = gameContext_->renderEngine_->getSpriteBatch();
    batch->begin();

    float tileWidth = gameContext_->viewWidth_;
    for(int renderIndex : renderTileList_){
        TerrainTile tile = tileData_[renderIndex];
        float tileX = renderIndex * tileWidth;
        float screenX = tileX - cam.camerLeft_;

        Rect dstRect;
        dstRect.left = screenX;
        dstRect.top = forestTop_;
        dstRect.width = tileWidth;
        dstRect.height = forestBgHeight_;

        if(tile.type == TERRAIN_TILE_TYPE_FOREST){
            auto forestRect = forestImage_->getRect();
            batch->renderImage(forestImage_ , forestRect, dstRect);
        }

        //render obstracer
        for(int i = 0 ; i < tile.obstraces->size() ; i++){
            auto obs = tile.obstraces->at(i);

            if(obs.state != Normal){
                continue;
            }
            Rect obsDstRect;
            obsDstRect.left = obs.position[0] - cam.camerLeft_;
            obsDstRect.top = obs.position[1];
            obsDstRect.width = obs.size[0];
            obsDstRect.height = obs.size[1];

            batch->renderRegionImage(*storeImageList_[obs.type] , obsDstRect);

            //debug
//            auto shapeBatch = gameContext_->renderEngine_->getShapeBatch();
//            shapeBatch->begin();
//            Paint paint;
//            paint.color = glm::vec4 (1.0f , 0.0f , 0.0f , 0.3f);
//            shapeBatch->renderRect(obsDstRect , paint);
//            shapeBatch->end();
        }//end for i

        auto srcRect = terrainImage_->getRect();
        dstRect.top = terrainHeight_;
        dstRect.width = tileWidth;
        dstRect.height = terrainHeight_;

        batch->renderImage(terrainImage_ , srcRect , dstRect);
    }//end for each

    batch->end();

    // Rect firstForestDstRect;
    // firstForestDstRect.left = firstTileX_;
    // firstForestDstRect.top = forestTop_;
    // firstForestDstRect.width = gameContext_->viewWidth_;
    // firstForestDstRect.height = forestBgHeight_;

    // Rect secondForestDstRect;
    // secondForestDstRect.left = secondTileX_;
    // secondForestDstRect.top = forestTop_;
    // secondForestDstRect.width = gameContext_->viewWidth_;
    // secondForestDstRect.height = forestBgHeight_;

    // batch->begin();
    // auto forestRect = forestImage_->getRect();
    // // forestRect.width = forestRect.height * ((forestDstRect.width / forestDstRect.height));
    // batch->renderImage(forestImage_ , forestRect, firstForestDstRect);
    // batch->renderImage(forestImage_ , forestRect, secondForestDstRect);
    // batch->end();

    // Rect firstTerrainDstRect;
    // firstTerrainDstRect.left = firstTileX_;
    // firstTerrainDstRect.top = terrainHeight_;
    // firstTerrainDstRect.width = gameContext_->viewWidth_;
    // firstTerrainDstRect.height = terrainHeight_;

    // Rect secondTerrainDstRect;
    // secondTerrainDstRect.left = secondTileX_;
    // secondTerrainDstRect.top = terrainHeight_;
    // secondTerrainDstRect.width = gameContext_->viewWidth_;
    // secondTerrainDstRect.height = terrainHeight_;
    // // srcRect.width = srcRect.height *(terrainDstRect.width / terrainDstRect.height);
    // batch->begin();
    // Rect srcRect = terrainImage_->getRect();
    // batch->renderImage(terrainImage_ , srcRect , firstTerrainDstRect);
    // batch->renderImage(terrainImage_ , srcRect , secondTerrainDstRect);
    // batch->end();

    //  std::cout << "forestBgHeight_ : " << forestBgHeight_
    //      << "  firstForestDstRect.width : " << firstForestDstRect.width
    //      << " firstForestDstRect.left " << firstForestDstRect.left
    //      << "  firstTileX_ " << firstTileX_ <<  std::endl;


    //for test
    // batch->begin();
    // Rect rect;
    // rect.left = 0.0f;
    // rect.top = terrainHeight_;
    // rect.width = 128.0f;
    // rect.height = terrainHeight_;

    // for(auto img : storeImageList_){
    //     batch->renderRegionImage(*img , rect);
    //     rect.left += rect.width + 20.0f;
    // }
    // batch->end();
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
