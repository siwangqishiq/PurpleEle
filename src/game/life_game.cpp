#include "life_game.hpp"
#include "../render/render_batch.hpp"
#include "../render/ui/view.hpp"
#include <memory>
#include "input/input_manager.hpp"
#include "../widget/timer.hpp"


/**
 * 
 * 康威生命游戏是一种细胞自动机，其规则如下：
// 1． 每个细胞的状态由该细胞及周围八个细胞上一次的状态所决定；
2. 如果一个细胞周围有3个细胞为生，则该细胞为生，即该细胞若原先为死，则转为生，若原先为生，则保持不变；
3. 如果一个细胞周围有2个细胞为生，则该细胞的生死状态保持不变；
4. 在其它情况下，该细胞为死，即该细胞若原先为生，则转为死，若原先为死，则保持不变
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

    const int cellNumPerRow = 40;
    
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
            
            rowVec.push_back(0);
        }//end for j
        cellData_.push_back(rowVec);
    }//end for i;

    iterCount_ = 0;

    //init data
    cellData_[0][24] = 1;
    cellData_[1][22] = 1;
    cellData_[1][24] = 1;

    cellData_[2][12] = 1;
    cellData_[2][13] = 1;
    cellData_[2][20] = 1;
    cellData_[2][21] = 1;
    cellData_[2][34] = 1;
    cellData_[2][35] = 1;
    
    cellData_[3][11] = 1;
    cellData_[3][15] = 1;
    cellData_[3][20] = 1;
    cellData_[3][21] = 1;
    cellData_[3][34] = 1;
    cellData_[3][35] = 1;

    cellData_[4][1] = 1;
    cellData_[4][0] = 1;
    cellData_[4][10] = 1;
    cellData_[4][16] = 1;
    cellData_[4][20] = 1;
    cellData_[4][21] = 1;

    cellData_[5][1] = 1;
    cellData_[5][0] = 1;
    cellData_[5][10] = 1;
    cellData_[5][14] = 1;
    cellData_[5][16] = 1;
    cellData_[5][17] = 1;
    cellData_[5][22] = 1;
    cellData_[5][24] = 1;

    cellData_[6][10] = 1;
    cellData_[6][16] = 1;
    cellData_[6][24] = 1;

    cellData_[7][11] = 1;
    cellData_[7][15] = 1;

    cellData_[8][12] = 1;
    cellData_[8][13] = 1;
}

void LifeGame::buildViews(){
    std::shared_ptr<TextView> titleTextView = std::make_shared<TextView>(gameZoneRect_.left , 
        viewHeight_ / 10.0f);

    rootView_->addView(titleTextView , 0 , -100);
    titleTextView->setTextSize(titleTextView->getViewRect().height / 2.0f);
    titleTextView->setTextGravity(Center);
    titleTextView->setText(L"进化次数");

    iterCountTextView_ = std::make_shared<TextView>(gameZoneRect_.left , viewHeight_ / 6.0f);
    iterCountTextView_->setTextSize(iterCountTextView_->getViewRect().height / 1.5f);
    iterCountTextView_->setTextGravity(Center);
    iterCountTextView_->setText(std::to_wstring(iterCount_));
    rootView_->addView(iterCountTextView_ , 0 , 
        -100 - titleTextView->getViewRect().height);

    
    auto countViewRect = iterCountTextView_->getViewRect();
    int reuleY = titleTextView->getViewRect().height + countViewRect.height + 100;

    ruleTextView_ = std::make_shared<TextView>(countViewRect.width , viewHeight_ - reuleY);
    ruleTextView_->setTextSize(countViewRect.width / 16.0f);
    ruleTextView_->setTextGravity(TopLeft);
    ruleTextView_->setText(L"康威生命游戏是一种细胞自动机,其规则如下:\n 1. 每个细胞的状态由该细胞及周围八个细胞上一次的状态所决定 \n2. 如果一个细胞周围有3个细胞为生,则该细胞为生,即该细胞若原先为死,则转为生,若原先为生,则保持不变 \n3. 如果一个细胞周围有2个细胞为生,则该细胞的生死状态保持不变 \n4. 在其它情况下,该细胞为死,即该细胞若原先为生,则转为死,若原先为死,则保持不变");
    // ruleTextView_->setBackgroundColor(glm::vec4(1.0f , 0.0f , 0.0f , 1.0f));
    rootView_->addView(ruleTextView_ , 0 , 
        -reuleY);

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

            //启动定时任务
            timerId_ = appContext->getTimer()->scheduleAtFixedRate([this](Application *app){
                // std::cout << "time task run " << std::endl;
                iterCount_++;
                iterOneStep();
                iterCountTextView_->setText(std::to_wstring(iterCount_));
            } , 250L);
        }else{ // 
            startButton_->setText(L"开始");
            //结束定时任务
            appContext->getTimer()->removeScheduleTask(timerId_);
            timerId_ = -1;
        }
        
        btnIsStart_ = !btnIsStart_;
        // iterOneStep();
    });

    stopButton_->setLambdaOnClickListener([this](View *view){
        Logi("LifeGame" , "Stop button click");

        appContext->getTimer()->removeScheduleTask(timerId_);
        timerId_ = -1;

        iterCount_ = 0;
        iterCountTextView_->setText(std::to_wstring(iterCount_));

        btnIsStart_ = true;
        startButton_->setText(L"开始");

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

/**
 * @brief 
    0 0 0 0 
    0 1 0 0 
    0 0 0 0
    0 0 0 0
 * 
 */
void LifeGame::iterOneStep(){
    auto originData = cellData_;
    for(int j = 0 ; j < cellRowCount_ ;j++){
        for(int i = 0 ; i < cellRowCount_;i++){
            int surroundCellCount = 0;
            
            if(j -1 >= 0 && i - 1 >= 0){ // left top
                if(originData[j - 1][ i - 1] > 0){
                    surroundCellCount++;
                }
            }
            if(j - 1 >= 0){ // top
                if(originData[j - 1][ i ] > 0){
                    surroundCellCount++;
                }
            }
            if(j - 1 >=0 && i + 1 < cellRowCount_){ // right top
                if(originData[j - 1][ i + 1] > 0){
                    surroundCellCount++;
                }
            }
            if(i - 1 >= 0){ // left
                if(originData[j][ i - 1] > 0){
                    surroundCellCount++;
                }
            }

            if(i + 1 < cellRowCount_){ // right
                if(originData[j][ i + 1] > 0){
                    surroundCellCount++;
                }
            }

            if( j + 1 < cellRowCount_ && i - 1 >= 0){
                if(originData[j + 1][ i - 1] > 0){
                    surroundCellCount++;
                }
            }

            if( j + 1 < cellRowCount_){
                if(originData[j + 1][i] > 0){
                    surroundCellCount++;
                }
            }

            if( j + 1 < cellRowCount_ && i + 1< cellRowCount_){
                if(originData[j + 1][i + 1] > 0){
                    surroundCellCount++;
                }
            }

            if(surroundCellCount == 3){
                cellData_[j][i] = 1;
            }else if(surroundCellCount == 2){
                cellData_[j][i] = cellData_[j][i];
            }else{
                cellData_[j][i] = 0;
            }
        }//end for j
    }//end for i;
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



