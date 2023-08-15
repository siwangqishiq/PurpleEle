#pragma once

#include <memory>
#include "../render.hpp"

class Drawable{
public:
    virtual void render(Rect &rect,std::shared_ptr<RenderEngine> renderEngine);
};

class NullDrawable : public Drawable{
public:
    NullDrawable(){
    }
};


class ColorDrawable : public Drawable{
public:
    ColorDrawable(glm::vec4 color){
        color_ = color;
    }
    
    virtual void render(Rect &rect,std::shared_ptr<RenderEngine> renderEngine);
private:
    glm::vec4 color_;
};

class RoundRectDrawable : public Drawable{
public:
    RoundRectDrawable(glm::vec4 color , float roundRadius){
        color_ = color;
        roundRadius_ = roundRadius;
    }
    
    virtual void render(Rect &rect,std::shared_ptr<RenderEngine> renderEngine);
private:
    glm::vec4 color_;
    float roundRadius_;
};

class ShadowRoundRectDrawable : public Drawable{
public:
    ShadowRoundRectDrawable(glm::vec4 color , float roundRadius , float blur){
        color_ = color;
        roundRadius_ = roundRadius;
        blur_ = blur;
    }
    
    virtual void render(Rect &rect,std::shared_ptr<RenderEngine> renderEngine);
private:
    glm::vec4 color_;
    float roundRadius_;
    float blur_;
};
