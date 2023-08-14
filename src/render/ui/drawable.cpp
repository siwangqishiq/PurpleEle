
#include "drawable.hpp"
#include "../render_batch.hpp"

void Drawable::render(Rect &rect,std::shared_ptr<RenderEngine> renderEngine){
    //do nothing
}

void ColorDrawable::render(Rect &rect,std::shared_ptr<RenderEngine> renderEngine){
    Paint paint;
    paint.color = this->color_;
    
    auto batch = renderEngine->getShapeBatch();
    batch->begin();
    batch->renderRect(rect, paint);
    batch->end();
}

void RoundRectDrawable::render(Rect &rect,std::shared_ptr<RenderEngine> renderEngine){
    Paint paint;
    paint.color = this->color_;

    auto batch = renderEngine->getShapeBatch();
    batch->begin();
    batch->renderRoundRect(rect , roundRadius_ , paint);
    batch->end();
}
