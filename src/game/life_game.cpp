#include "life_game.hpp"
#include "../render/render_batch.hpp"
#include "../render/ui/view.hpp"
#include <memory>


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
    buildViews();

    if(isInited){
        return;
    }
    isInited = true;
}

void LifeGame::buildViews(){
    // rootView_->setBackgroundColor(glm::vec4(1.0f ,1.0f ,1.0f ,1.0f));

    gameZoneRect_.width = viewHeight_;
    gameZoneRect_.height = viewHeight_;
    gameZoneRect_.left = viewWidth_ / 2 - gameZoneRect_.width / 2;
    gameZoneRect_.top = gameZoneRect_.height;

    cellSize_ = gameZoneRect_.width / 200;
    cellRowCount_ = gameZoneRect_.width / cellSize_;

    cellData_ = std::vector<std::vector<char>>();

    for(int i = 0 ; i < cellRowCount_ ;i++){
        std::vector<char> rowVec;
        for(int j = 0 ; j < cellRowCount_;j++){
            if(i == j){
                rowVec.push_back(1);
            }else{
                rowVec.push_back(0);
            }
        }//end for j
        cellData_.push_back(rowVec);
    }//end for i;
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

            if(cellData_[i][j] == 0){
                cellPaint.fillStyle = Stroken;
                batch->renderRect(cellRect , cellPaint);
            }else{
                cellPaint.fillStyle = Filled;
                batch->renderRect(cellRect , cellPaint);
            }
          
           x += cellSize_;
        }//end for j
        x = gameZoneRect_.left;
        y -= cellSize_;
    }//end for i;

    batch->end();
    // limitCount=100;
}

void LifeGame::dispose(){

}

bool LifeGame::onEventAction(int action , float x , float y){
    bool cost = rootView_->dispatchTouchEvent(action , x , y);
    if(!cost){
    }
    return cost;
}



