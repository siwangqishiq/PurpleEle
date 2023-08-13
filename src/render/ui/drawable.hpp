#pragma once

#include <memory>
#include "../render.hpp"

class Drawable{
public:
    virtual void render(RectI rect,std::shared_ptr<RenderEngine> renderEngine);
};


class ColorDrawable : public Drawable{
public:
    ColorDrawable(glm::vec4 color){
        color_ = color;
    }
    
    virtual void render(RectI rect,std::shared_ptr<RenderEngine> renderEngine);
private:
    glm::vec4 color_;
};