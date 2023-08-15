
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

void ShadowRoundRectDrawable::render(Rect &rect,std::shared_ptr<RenderEngine> renderEngine){
    Paint paint;

    auto batch = renderEngine->getShapeBatch();
    batch->begin();
    //shadow
    Rect shadowRect = rect;
    // paint.color = glm::vec4(0.0f ,0.0f ,0.0f ,1.0f);
    paint.color = this->color_;
    shadowRect.left =  shadowRect.left + blur_;
    shadowRect.top = shadowRect.top - blur_;
    shadowRect.width = shadowRect.width - 2 * blur_;
    shadowRect.height = shadowRect.height - 2 * blur_;
    shadowRect.left += shadowRect.width * 0.005f;
    shadowRect.top -= shadowRect.height * 0.01f;
    // paint.color = glm::vec4(0.0f ,0.0f ,0.0f ,0.4f);
    batch->renderBlurRoundRect(shadowRect , roundRadius_ , 2*blur_ , paint);

    paint.color = this->color_;
    batch->renderRoundRect(rect , roundRadius_ , paint);
    batch->end();
}
