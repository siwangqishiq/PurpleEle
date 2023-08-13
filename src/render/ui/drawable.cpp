
#include "drawable.hpp"
#include "../render_batch.hpp"

void Drawable::render(RectI rect,std::shared_ptr<RenderEngine> renderEngine){
    //do nothing
}

void ColorDrawable::render(RectI rect,std::shared_ptr<RenderEngine> renderEngine){
    Paint paint;
    paint.color = this->color_;
    
    auto batch = renderEngine->getShapeBatch();
    batch->begin();
    batch->renderRect(rect.toRectF(), paint);
    batch->end();
}