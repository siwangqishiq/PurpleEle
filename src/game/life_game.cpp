#include "life_game.hpp"
#include "../render/render_batch.hpp"
#include "../render/ui/view.hpp"
#include <memory>
#include "input/input_manager.hpp"


/**
 * 
 * 康威生命游戏是一种细胞自动机，其规则如下：
// 每个细胞有两种状态：存活或死亡。
// 每个细胞的状态由其周围八个细胞的状态决定。
// 如果一个细胞周围有三个存活的细胞，则该细胞在下一个时间步骤中将变为存活状态（或保持存活状态）。
// 如果一个细胞周围有少于两个存活的细胞，则该细胞在下一个时间步骤中将死亡（或保持死亡状态）。
// 如果一个细胞周围有超过三个存活的细胞，则该细胞在下一个时间步骤中将死亡（或保持死亡状态）。
// 这些规则被应用于整个细胞自动机，以模拟生命在一个二维网格中的演化。
 * 
 * 
 */

void LifeGame::init(){
    viewWidth_ = appContext->viewWidth_;
    viewHeight_ = appContext->viewHeight_;

    renderEngine_ = appContext->getRender();
    appContext->addEventActionCallback(this);
    appContext->isShowFps = true;
    
    //
    rootView_ = appContext->getRootView();

    gameInit();
    buildViews();

    if(isInited){
        return;
    }
    isInited = true;
}

void LifeGame::gameInit(){
    gameZoneRect_.width = viewHeight_;
    gameZoneRect_.height = viewHeight_;
    gameZoneRect_.left = viewWidth_ / 2 - gameZoneRect_.width / 2;
    gameZoneRect_.top = gameZoneRect_.height;

    const int cellNumPerRow = 30;
    
    cellSize_ = gameZoneRect_.width / cellNumPerRow;
    cellRowCount_ = gameZoneRect_.width / cellSize_;

    cellData_ = std::vector<std::vector<char>>();

    for(int i = 0 ; i < cellRowCount_ ;i++){
        std::vector<char> rowVec;
        for(int j = 0 ; j < cellRowCount_;j++){
            // if(i == j){
            //     rowVec.push_back(1);
            // }else{
            //     rowVec.push_back(0);
            // }
           
            if(i == 0 && j == 0){
                rowVec.push_back(1);
            }else{
                rowVec.push_back(0);
            }
        }//end for j
        cellData_.push_back(rowVec);
    }//end for i;

    iterCount_ = 0;
}

void LifeGame::buildViews(){
    std::shared_ptr<TextView> titleTextView = std::make_shared<TextView>(gameZoneRect_.left , 
        viewHeight_ / 10.0f);

    rootView_->addView(titleTextView , 0 , -100);
    titleTextView->setTextSize(titleTextView->getViewRect().height / 2.0f);
    titleTextView->setTextGravity(Center);
    titleTextView->setText(L"进化次数");

    iterCountTextView_ = std::make_shared<TextView>(gameZoneRect_.left , 
        viewHeight_ / 5.0f);
    iterCountTextView_->setTextSize(iterCountTextView_->getViewRect().height / 1.5f);
    iterCountTextView_->setTextGravity(Center);
    iterCountTextView_->setText(std::to_wstring(iterCount_));
    rootView_->addView(iterCountTextView_ , 0 , 
        -100 - titleTextView->getViewRect().height);

    int btnPadding = 10;
    int btnWidth = viewWidth_ - gameZoneRect_.left - gameZoneRect_.width - btnPadding * 2;
    int btnHeight = viewHeight_ / 10;

    startButton_ = std::make_shared<ButtonView>(btnWidth , btnHeight , L"开始");
    // startButton_->setText(L"开始");
    rootView_->addView(startButton_ ,  
        gameZoneRect_.left + gameZoneRect_.width + btnPadding ,
        -100);

    stopButton_ = std::make_shared<ButtonView>(btnWidth , btnHeight , L"停止进化");
    rootView_->addView(stopButton_ ,  
        gameZoneRect_.left + gameZoneRect_.width + btnPadding ,
        - startButton_->getViewRect().height - 100 - 20);

    startButton_->setLambdaOnClickListener([this](View *view){
        Logi("LifeGame" , "Start button click");
        
        if(btnIsStart_){ // pause
            startButton_->setText(L"暂停");
        }else{
            startButton_->setText(L"开始");
        }
        
        btnIsStart_ = !btnIsStart_;
    });

    stopButton_->setLambdaOnClickListener([this](View *view){
        Logi("LifeGame" , "Stop button click");
        resetCellData();
    });
}

void LifeGame::tick(){
    rootView_->onRender(renderEngine_); //UI render

    //  static int limitCount = 100;

    Paint paint;
    paint.color = COLOR_BLACK;

    auto batch = renderEngine_->getShapeBatch();
    batch->begin();
    batch->renderRect(gameZoneRect_ , paint);

    Paint cellPaint;
    cellPaint.color = COLOR_GREEN;
    cellPaint.stokenWidth = 1.0f; 

    float x = gameZoneRect_.left;
    float y = gameZoneRect_.top;

    Rect cellRect;
    cellRect.width = cellSize_;
    cellRect.height = cellSize_;

    // int drawCount = 0;
    for(int i = 0 ; i < cellRowCount_ ;i++){
        for(int j = 0 ; j < cellRowCount_;j++){
            cellRect.left = x;
            cellRect.top = y;
            
            // cellPaint.color = glm::vec4(GenRandomFloat() , GenRandomFloat() , 
            //     GenRandomFloat() ,1.0f);

            if(cellData_[i][j] == 1){
                // cellPaint.color = COLOR_GREEN;
                cellPaint.fillStyle = Filled;
                batch->renderRect(cellRect , cellPaint);
            }else{
                // cellPaint.color = COLOR_WHITE;
                cellPaint.fillStyle = Stroken;
                batch->renderRect(cellRect , cellPaint);
            }
          
           x += cellSize_;
        }//end for j
        x = gameZoneRect_.left;
        y -= cellSize_;
    }//end for i;

    batch->end();
    // limitCount=100;

    // batch->begin();
    // Paint circlePaint;
    // circlePaint.color = COLOR_GRAY;
    // batch->renderBlurCircle(100,100,200,20 , circlePaint);
    // batch->end();
}

void LifeGame::dispose(){

}

bool LifeGame::onEventAction(int action , float x , float y){
    bool cost = rootView_->dispatchTouchEvent(action , x , y);
    if(!cost){
        if(gameZoneRect_.isPointInRect(x , y)){
            // std::cout << "point in game zone!" << std::endl;
            handleOnEventInGame(action , x, y);
        }
    }
    return cost;
}

void LifeGame::resetCellData(){
    for(int j = 0 ; j < cellRowCount_ ;j++){
        for(int i = 0 ; i < cellRowCount_;i++){
            cellData_[j][i] = 0;
        }//end for j
    }//end for i;
}

void LifeGame::handleOnEventInGame(int event , float x , float y){
    if(event != ACTION_DOWN){
        return;
    }

    float startX = gameZoneRect_.left;
    float startY = gameZoneRect_.top;

    int idx = static_cast<int>((x - startX) / cellSize_);
    int idy = static_cast<int>((startY - y) / cellSize_);
    std::cout << "indexX = " << idx << " indexY = " << idy << std::endl;
    if(idx >=0 && idx < cellRowCount_ && idy >=0 && idy < cellRowCount_){ 
        //change array data
        if(cellData_[idy][idx] == 0){
            cellData_[idy][idx] = 1;
        }else{
            cellData_[idy][idx] = 0;
        }
    }
}



