#pragma once

#include "log.hpp"
#include "../common.hpp"
#include <memory>
#include <vector>
#include <functional>
#include "drawable.hpp"

class RenderEngine;
class ViewGroup;
class View;
class RootViewGroup;

const int VIEW_VISIBLE = 1;
const int VIEW_GONE = 0;
const int VIEW_INVISIBLE = -1;

const glm::vec4 COLOR_WHITE(1.0f , 1.0f ,1.0f ,1.0f);
const glm::vec4 COLOR_BLACK(0.0f , 0.0f ,0.0f ,1.0f);
const glm::vec4 COLOR_GRAY(0.5f , 0.5f , 0.5f ,1.0f);

enum ViewState{
    IDLE,
    PRESSED
};

class IViewListener{
public:
    virtual void onClick(View *view) = 0;
};

//UI base class
class View{
public:
    View(int w , int h);

    virtual void onRender(std::shared_ptr<RenderEngine> renderEngine);

    virtual void setViewPosition(int left , int top);

    virtual void setViewSize(int width , int height);

    virtual bool dispatchTouchEvent(int action , float x , float y);

    virtual bool onTouchEvent(int action , float x , float y);

    virtual bool interceptTouchEvent(int action , float x , float y){
        return true;
    }

    virtual void setCustomRenderPass(std::function<void(std::shared_ptr<RenderEngine> renderEngine)> 
        renderPass){
        this->customRenderPass_ = renderPass;
    }

    int visible = VIEW_VISIBLE;

    RectI getViewRect() {
        return viewRect_;
    }

    // user interface
    // set View background color;
    virtual void setBackgroundColor(glm::vec4 bgColor);

    virtual void setBackgroundRoundRect(glm::vec4 bgColor , float radius);

    virtual void setBackgroundShadowRoundRect(glm::vec4 bgColor , float radius , 
                    float blur);

    void setParentView(ViewGroup *parentView){
        parentView_ = parentView;
    }

    void setOnClickListener(IViewListener *onClickListener){
        onClickListener_ = onClickListener;
    }

    virtual void setLambdaOnClickListener(std::function<void(View *)> callback){
        lambdaClickCallback_ = callback;
    }

    virtual void setViewStateChangeListener(std::function<void(View *, ViewState)> callback){
        viewStateChangeCallback_ = callback;
    }

    virtual ViewState getViewState(){
        return state_;
    }
    
    void setTag(std::string tag){
        tag_ = tag;
    }

    RootViewGroup* findRootView();

    ~View(){
        Logi("ui_view" , "~ free view %s" , tag_.c_str());
    }

    std::string tag_;
protected:
    RectI viewRect_;

    ViewGroup *parentView_ = nullptr;
    Paint backgroundPaint_;

    //click action callback
    IViewListener *onClickListener_ = nullptr;

    ViewState state_ = IDLE;

    std::shared_ptr<Drawable> backgroundDrawable_ = nullptr;

    virtual bool isRootHasTarget();

    virtual bool hasActionCallback(); 

private:
    RootViewGroup *rootViewCached = nullptr;

    std::function<void(View *)> lambdaClickCallback_ = nullptr;

    std::function<void(View *, ViewState)> viewStateChangeCallback_ = nullptr;

    std::function<void(std::shared_ptr<RenderEngine> renderEngine)> customRenderPass_ = nullptr;
};

//ViewGroup
class ViewGroup : public View{
public:
    ViewGroup(int viewWidth, int viewHeight) : View(viewWidth , viewHeight){
        // Logi("ui_view" , "create ViewGroup");
    }

    virtual void clearAllChildViews();

    virtual void onRender(std::shared_ptr<RenderEngine> renderEngine);

    virtual void renderSelf(std::shared_ptr<RenderEngine> renderEngine);
    
    virtual void addView(std::shared_ptr<View> view , int offsetX , int offsetY);

    virtual bool dispatchTouchEvent(int action , float x , float y);
    
    virtual bool onTouchEvent(int action , float x , float y);

    virtual bool interceptTouchEvent(int action , float x , float y){
        return false;
    }

    ~ViewGroup(){
        Logi("ui_view" , "~ free viewgroup %s" , tag_.c_str());
    }
protected:
    std::vector<std::shared_ptr<View>> childViews_;

};

class RootViewGroup : public ViewGroup{
public:
    RootViewGroup(int viewWidth, int viewHeight) : ViewGroup(viewWidth , viewHeight){
    }

    virtual bool dispatchTouchEvent(int action , float x , float y);

    View *targetView_ = nullptr;
};

class TextView : public View {
public:
    TextView(int viewWidth, int viewHeight) : View(viewWidth , viewHeight){
    }

    virtual void setTextSize(float textSize);

    virtual void setTextColor(glm::vec4 textColor);

    virtual void setTextGravity(TextGravity gravity);

    virtual void setText(std::wstring text);
    
    virtual void onRender(std::shared_ptr<RenderEngine> renderEngine);
protected:
    TextPaint textPaint_;
    std::wstring text_;
};

class ButtonView : public TextView {
public:
    ButtonView(int viewWidth, int viewHeight , std::wstring text) 
        : TextView(viewWidth , viewHeight){
        this->text_ = text;
        initButton();
    }

    virtual void setButtonUI(glm::vec4 color , float radius , float blur);
protected:
    std::shared_ptr<Drawable> idleBackgroundDrawable_ = nullptr;
    std::shared_ptr<Drawable> pressedBackgroundDrawable_ = nullptr;
private:
    void initButton();
};


