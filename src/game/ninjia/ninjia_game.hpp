#pragma once

#include "application.hpp"
#include "utils.hpp"
#include "render/ui/view.hpp"
#include <memory>
#include "ninjia_game_base.hpp"
#include "ninjia_sprite.hpp"
#include "ninjia_background.hpp"

class NinjiaGame;
class TextureImage;
class Camera;
class NinjiaPlayer;

enum GameState{
    Splash, //
    Running,
    End
};

class NinjiaGame : public IScene,EventActionCallback {
public:
    NinjiaGame(Application *app){
        appContext = app;
    }

    GameState gameState_;

    bool isInited = false;
    
    virtual void init() override;

    virtual void tick() override;

    virtual void dispose() override;

    virtual bool onEventAction(int action , float x , float y) override;

    void buildViews();

    void gameInit();

    void gameStartPrepare();

    void renderSplash();

    void runningUpdate();
    void renderRunning();

    void renderHud();

    void renderEnded();
    
    void renderNinjaDistanceHud();

    void renderScore();

    void playerGetScore(int addScore);

    const std::string AUDIO_HIT = "hit";
    const std::string AUDIO_JUMP = "jump";
    const std::string AUDIO_BGM = "bgm";
    const std::string AUDIO_STONE_HIT = "stone_hit";
    const std::string AUDIO_GAME_OVER = "game_over";

    int viewWidth_;
    int viewHeight_;

    std::shared_ptr<RenderEngine> renderEngine_;

    std::shared_ptr<Camera> camera_;
    std::shared_ptr<SkyBackground> skybg_;
    std::shared_ptr<Terrain> terrain_;
    std::shared_ptr<NinjiaPlayer> player_;
private:
    Application *appContext;

    std::shared_ptr<TextureImage> splashImage_;
    float splashDeltaTime_ = 0.0f;
    float splashTextAlpha_ = 1.0f;
    bool splashIsPressed = false;

    TextPaint distanceHudTextPaint;

    int currentScore_ = 0;
    int targetScore_ = 0;

    TextPaint scoreTextPaint;
};