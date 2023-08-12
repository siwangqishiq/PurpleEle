#pragma once

#include "log.hpp"
#include "../common.hpp"
#include <memory>
#include <vector>
#include <functional>

class RenderEngine;
class ViewGroup;
class View;
class RootViewGroup;

const int VIEW_VISIBLE = 1;
const int VIEW_GONE = 0;
const int VIEW_INVISIBLE = -1;

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

    int visible = VIEW_VISIBLE;

    RectI getViewRect() {
        return viewRect_;
    }

    // user interface
    // set View background color;
    virtual void setBackgroundColor(glm::vec4 bgColor);

    void setParentView(ViewGroup *parentView){
        parentView_ = parentView;
    }

    void setOnClickListener(IViewListener *onClickListener){
        onClickListener_ = onClickListener;
    }

    void setLambdaOnClickListener(std::function<void(View *)> callback){
        lambdaClickCallback_ = callback;
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
    
    virtual bool isRootHasTarget();

    virtual bool hasActionCallback(); 
private:
    RootViewGroup *rootViewCached = nullptr;

    std::function<void(View *)> lambdaClickCallback_ = nullptr;
};

//ViewGroup
class ViewGroup : public View{
public:
    ViewGroup(int viewWidth, int viewHeight) : View(viewWidth , viewHeight){
        // Logi("ui_view" , "create ViewGroup");
    }

    virtual void onRender(std::shared_ptr<RenderEngine> renderEngine);

    virtual void renderSelf(std::shared_ptr<RenderEngine> renderEngine);
    
    virtual void addView(std::shared_ptr<View> view , int offsetX , int offsetY);

    virtual bool dispatchTouchEvent(int action , float x , float y);
    
    virtual bool onTouchEvent(int action , float x , float y);

    virtual bool interceptTouchEvent(int action , float x , float y){
        return false;
    }

    ~ViewGroup(){
        View::~View();
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


