#include "ninjia_game.hpp"
#include "render/render_batch.hpp"
#include "render/ui/view.hpp"
#include "resource/asset_manager.hpp"
#include "render/sprite.hpp"
#include "render/texture.hpp"
#include <memory>
#include "render/common.hpp"
#include "input/input_manager.hpp"
#include "audio/audio.hpp"
#include "widget/timer.hpp"
#include "ninjia_background.hpp"
#include "ninjia_sprite.hpp"


void NinjiaGame::init(){
    viewWidth_ = appContext->viewWidth_;
    viewHeight_ = appContext->viewHeight_;

    renderEngine_ = appContext->getRender();
    appContext->addEventActionCallback(this);
    appContext->isShowFps = true;
    
    //
    gameInit();

    if(isInited){
        return;
    }
    isInited = true;
}

void NinjiaGame::gameInit(){

    camera_ = std::make_shared<Camera>(this);
    skybg_ = std::make_shared<SkyBackground>(this);
    terrain_ = std::make_shared<Terrain>(this);
    player_ = std::make_shared<NinjiaPlayer>(this);

    camera_->init();
    skybg_->init();
    terrain_->init();
    player_->init();

    splashImage_ = BuildImageByAsset("sprite/splash.png");
    splashIsPressed = false;
    splashDeltaTime_ = 0.0f;
    AudioManager::getInstance()->loadAudio("audio/hit.mp3",AUDIO_HIT);
    AudioManager::getInstance()->loadAudio("audio/pao.mp3",AUDIO_BGM);

    gameState_ = Running;
}

void NinjiaGame::gameStartPrepare(){
    // AudioManager::getInstance()->playAudio(AUDIO_BGM);
}

void NinjiaGame::buildViews(){
    //do nothing
}

void NinjiaGame::tick(){
    switch(gameState_){
        case Splash:
        renderSplash();
        break;
        case Running:
        renderRunning();
        break;
    }//end switch
}

void NinjiaGame::dispose(){
    if(skybg_ != nullptr){
        skybg_->dispose();
    }
    if(terrain_ != nullptr){
        terrain_->dispose();
    }
    if(player_ != nullptr){
        player_->dispose();
    }
    // AudioManager::getInstance()->dispose();
}

bool NinjiaGame::onEventAction(int action , float x , float y){
    switch(gameState_){
        case Splash: 
        if(action == ACTION_DOWN){
            AudioManager::getInstance()->playAudio(AUDIO_HIT);
            if(splashIsPressed){
               return true; 
            }
            splashIsPressed = true;

            //start Timer 3s enter game main loop
            appContext->getTimer()->schedule([this](Application *app){
                gameState_ = Running;
                splashIsPressed = false;
                gameStartPrepare();
            } , 3000);
        }
        break;
    }//end switch

    return false;
}

void NinjiaGame::renderRunning(){
    skybg_->render();// sky background render
    terrain_->render();
    player_->render();
}

void NinjiaGame::renderSplash(){
    Rect screenRect;
    screenRect.left = 0.0f;
    screenRect.top = viewHeight_;
    screenRect.width = viewWidth_;
    screenRect.height = viewHeight_;

    auto batch = renderEngine_->getSpriteBatch();
    batch->begin();
    Rect splashSrcRect = splashImage_->getRect();
    batch->renderImage(splashImage_ , splashSrcRect , screenRect);
    batch->end();

    splashTextAlpha_ = glm::sin(splashDeltaTime_) + 1.0f;
    if(splashIsPressed){
        splashDeltaTime_ += 0.5f;
    }else{
        splashDeltaTime_ += 0.04f;
    }

    Rect textRect;
    textRect.left = 0.0f;
    textRect.top = viewHeight_ / 4.0f;
    textRect.width = viewWidth_;
    textRect.height = viewHeight_ / 4.0f;

    TextPaint textPaint;
    textPaint.textColor = glm::vec4(1.0f , 1.0f , 1.0f , splashTextAlpha_);
    textPaint.textGravity= TextGravity::Center;
    textPaint.setTextSize(textRect.height * 0.4f);
    renderEngine_->renderTextWithRect(L"按下任意键开始" , textRect , textPaint , nullptr);
}


