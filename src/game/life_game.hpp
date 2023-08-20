#pragma once

#include "application.hpp"
#include "utils.hpp"
#include "render/ui/view.hpp"
#include <memory>
#include <vector>

class LifeGame : public IScene,EventActionCallback {
public:
    LifeGame(Application *app){
        appContext = app;
    }

    glm::vec4 COLOR_SKY_BLUE = ConvertColor(33,150,243,255);
    glm::vec4 COLOR_WHITE = ConvertColor(255,255,255,255);
    glm::vec4 COLOR_GRAY = ConvertColor(115 , 115 , 115 , 255);

    bool isInited = false;
    
    virtual void init() override;

    virtual void tick() override;

    virtual void dispose() override;

    virtual bool onEventAction(int action , float x , float y) override;

    void gameInit();
    
    void buildViews();

    void resetCellData();

    void iterOneStep();

    int timerId_ = -1;
private:
    Application *appContext;

    int viewWidth_;
    int viewHeight_;

    int iterCount_ = 0;//进化次数

    std::shared_ptr<RenderEngine> renderEngine_;
    
    //UI root view
    std::shared_ptr<RootViewGroup> rootView_;

    RectI gameZoneRect_;
    int cellSize_;
    int cellRowCount_ = 0;

    bool btnIsStart_ = true;
    std::vector<std::vector<char>> cellData_;

    std::shared_ptr<TextView> iterCountTextView_;

    std::shared_ptr<ButtonView> startButton_;
    std::shared_ptr<ButtonView> stopButton_;

    void handleOnEventInGame(int event , float x , float y);
};