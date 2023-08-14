
#include "view.hpp"
#include "../render.hpp"
#include "../render_batch.hpp"
#include "../../input/input_manager.hpp"

View::View(int w , int h){
    viewRect_.left = 0.0f;
    viewRect_.top = h;
    viewRect_.width = w;
    viewRect_.height = h;
}

void View::setViewPosition(int left , int top){
    if(parentView_ != nullptr){
        auto parentViewRect = parentView_->getViewRect();
        viewRect_.left = parentViewRect.left + left;
        viewRect_.top = parentViewRect.top += top;
    }else{
        viewRect_.left = left;
        viewRect_.top = top;
    }
}

void View::setViewSize(int width , int height){
    viewRect_.width = width;
    viewRect_.height = height;
}

void View::setBackgroundColor(glm::vec4 bgColor){
    backgroundDrawable_ = std::make_shared<ColorDrawable>(bgColor);
}

void View::setBackgroundRoundRect(glm::vec4 bgColor , float radius){
    backgroundDrawable_ = std::make_shared<RoundRectDrawable>(bgColor , radius);
}

bool View::dispatchTouchEvent(int action , float x , float y){
    return onTouchEvent(action , x , y);
}
 
bool View::onTouchEvent(int action , float x , float y){
    // Logi("view" , "%s view onTouchEvent %d (%f , %f)" ,tag_.c_str(),
    //      action , x, y);
    if(!hasActionCallback()){
        return false;
    }

    bool result = false;
    switch(action){
        case ACTION_DOWN:
        result = true;
        break;
        case ACTION_MOVE:
        result = true;
        break;
        case ACTION_UP:
        result = true;
        if(onClickListener_ != nullptr){
            onClickListener_->onClick(this);
        }

        if(lambdaClickCallback_ != nullptr){
            lambdaClickCallback_(this);
        }
        break;
        default:
        break;
    }//end switch

    return result;
}

bool View::hasActionCallback(){
    return onClickListener_ != nullptr || lambdaClickCallback_ != nullptr;
}

//是否rootView已经设置了targetView
bool View::isRootHasTarget(){
    auto rootView = this->findRootView();
    return rootView != nullptr && rootView->targetView_ != nullptr;
}

void View::onRender(std::shared_ptr<RenderEngine> renderEngine){
    if(visible != VIEW_VISIBLE){
        return;
    }

    // Logi("view" , "view on render %d , %d , %d , %d" , viewRect_.left , viewRect_.top
    //     ,viewRect_.width , viewRect_.height);
    
    Rect viewRectF = viewRect_.toRectF();
    //draw background
    if(backgroundDrawable_ != nullptr){
        backgroundDrawable_->render(viewRectF , renderEngine);
    }

    // auto batch = renderEngine->getShapeBatch();
    // batch->begin();
    // batch->renderRect(viewRect_, backgroundPaint_);
    // batch->end();

    //自定义渲染流程
    if(customRenderPass_ != nullptr){
        customRenderPass_(renderEngine);
    }
}

void ViewGroup::onRender(std::shared_ptr<RenderEngine> renderEngine){
    //todo render self
    renderSelf(renderEngine);
    
    //render child
    for(auto &childView : childViews_){
        childView->onRender(renderEngine);
    }//end for each
}

void ViewGroup::renderSelf(std::shared_ptr<RenderEngine> renderEngine){
    View::onRender(renderEngine);
}

void ViewGroup::addView(std::shared_ptr<View> view , int offsetX , int offsetY){
    view->setParentView(this);
    view->setViewPosition(offsetX , offsetY);

    childViews_.push_back(view);
}

bool ViewGroup::dispatchTouchEvent(int action , float x , float y){
    for(int i = childViews_.size() - 1 ; i >= 0 ;i--){
        auto childView = childViews_[i];
        if(childView->getViewRect().isPointInRect(x , y)){
            // Logi("view" , "%s viewgroup dispathTouchEvent %d (%f , %f)" 
            //     ,childView->tag_.c_str(),
            //     action , x, y);
            if(childView->dispatchTouchEvent(action , x , y)){
                if(!isRootHasTarget()){
                    auto rootView = childView->findRootView();
                    if(rootView != nullptr){
                        // std::cout << "set target view -> " << childView->tag_ << std::endl;
                        rootView->targetView_ = childView.get();
                        // std::cout << "isRootHasTarget() -> " << isRootHasTarget() << std::endl;
                    }
                }
                return true;
            }//end if
        }
    }//end for i

    if(isRootHasTarget()){
        return true;
    }
    return onTouchEvent(action , x , y);
}

RootViewGroup* View::findRootView(){
    if(rootViewCached != nullptr){
        return rootViewCached;
    }

    View *p = this;
    View *prior = p;
    while(p != nullptr){
        prior = p;
        p = p->parentView_;
    }//end while
    rootViewCached = static_cast<RootViewGroup*>(prior);
    return rootViewCached;
}

bool ViewGroup::onTouchEvent(int action , float x , float y){
    return View::onTouchEvent(action , x , y);
}

bool RootViewGroup::dispatchTouchEvent(int action , float x , float y){
    if(action == ACTION_UP){//clear action session
        if(targetView_ != nullptr){
            targetView_->onTouchEvent(action , x , y);
        }
        targetView_ = nullptr;
        // std::cout << "clear target view!!!" << std::endl;
        return true;
    }

    if(targetView_ != nullptr){
        // std::cout << "target view is " << targetView_->tag_ << std::endl;
        return targetView_->onTouchEvent(action , x , y);
    }
    
    if(action == ACTION_DOWN){
        return ViewGroup::dispatchTouchEvent(action , x , y);
    }

    return false;
}

void TextView::setTextSize(float textSize){
    textPaint_.setTextSize(textSize);
}

void TextView::setTextColor(glm::vec4 textColor){
    textPaint_.textColor = textColor;
}

void TextView::setTextGravity(TextGravity gravity){
    textPaint_.textGravity = gravity;
}

void TextView::setText(std::wstring text){
    text_ = text;
}

void TextView::onRender(std::shared_ptr<RenderEngine> renderEngine){
    View::onRender(renderEngine);
    auto rect = viewRect_.toRectF();
    renderEngine->renderTextWithRect(text_ , rect , textPaint_, nullptr);
}

