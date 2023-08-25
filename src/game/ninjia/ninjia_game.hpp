#pragma once

#include "application.hpp"
#include "utils.hpp"
#include "render/ui/view.hpp"
#include <memory>

class TextureImage;

enum GameState{
    Splash, //
    Running,
    End
};

class Player{

};

class Camera{

};

class NinjiaGame : public IScene,EventActionCallback {
public:
    NinjiaGame(Application *app){
        appContext = app;
    }

    GameState gameState_ = Splash;

    bool isInited = false;
    
    virtual void init() override;

    virtual void tick() override;

    virtual void dispose() override;

    virtual bool onEventAction(int action , float x , float y) override;

    void buildViews();

    void gameInit();

    void gameStartPrepare();

    void renderSplash();

    const std::string AUDIO_HIT = "hit";

    const std::string AUDIO_BGM = "bgm";
private:
    Application *appContext;

    int viewWidth_;
    int viewHeight_;

    std::shared_ptr<RenderEngine> renderEngine_;

    std::shared_ptr<TextureImage> splashImage_;
    float splashDeltaTime_;
    float splashTextAlpha_ = 0.0f;
    bool splashIsPressed = false;
};