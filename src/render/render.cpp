#include "render.hpp"
#include "application.hpp"
#include "log.hpp"
#include "command.hpp"
#include "vram.hpp"
#include "texture.hpp"
#include "resource/asset_manager.hpp"
#include "../libjson/json.hpp"
#include "glheader.hpp"
#include "application.hpp"
#include "render_batch.hpp"
#include "vram.hpp"

std::unordered_map<wchar_t , wchar_t> SymbolMap;

void RenderEngine::render(){
    // glClearColor(0.0f , 1.0f , 0.0f , 1.0f);
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_DEPTH_TEST);
    
    //gl render
    for(auto &cmd : renderCommandList_){
        cmd->runCommands();
        cmd->used = false;
    }
    
    //clear cmd list
    clearRenderCommands();
    vramManager_->onPostRender();
}

void RenderEngine::free(){
    if(shapeBatch_ != nullptr){
        shapeBatch_->dispose();
    }

    // VRamManager::getInstance()->clear();
    vramManager_->clear();
    ShaderManager::getInstance()->clear();
    TextureManager::getInstance()->clear();
}

void RenderEngine::clearRenderCommands(){
    resetDepth();
    renderCommandList_.clear();
}

void RenderEngine::onScreenResize(){
//    viewWidth_ = appContext_->viewWidth_;
//    viewHeight_ = appContext_->viewHeight_;
    resetNormalMat(appContext_->viewWidth_ , appContext_->viewHeight_);
    glViewport(0 , 0 , appContext_->viewWidth_ , appContext_->viewHeight_);
}

void RenderEngine::init(){
    Logi(TAG , "render engine init start");

    vramManager_ = std::make_shared<VRamManager>();

    loadTextRenderResource();//text render init
    // Logi(TAG , "render engine init end");
    
    loadShapeShader();
    shapeBatch_ = std::make_shared<ShapeBatch>(this);//std::shared_ptr<RenderEngine>(this)
    shapeBatch_->init();

    spriteBatch_ = std::make_shared<SpriteBatch>(this);
    spriteBatch_->init();

    clearRenderCommands();
}

void RenderEngine::loadTextRenderResource(){
    Logi(TAG , "render init loadTextRenderResource");
    textRenderHelper_ = std::make_shared<TextRenderHelper>();
    textRenderHelper_->loadRes(*this);

    TextRenderHelper::loadSymbolMap();
}

void RenderEngine::loadShapeShader(){
    Logi(TAG , "render init loadShapeShader");
    
    // ShaderManager::getInstance()->loadAssetShader("shape_rect" , 
    //     "shader/shape_vert.glsl", "shader/shape_rect_frag.glsl");
    // ShaderManager::getInstance()->loadAssetShader("shape_circle" , 
    //     "shader/shape_vert.glsl", "shader/shape_circle_frag.glsl");
    // ShaderManager::getInstance()->loadAssetShader("shape_oval" , 
    //     "shader/shape_vert.glsl", "shader/shape_oval_frag.glsl");
    // ShaderManager::getInstance()->loadAssetShader("shape_round_rect" , 
    //     "shader/shape_vert.glsl", "shader/shape_round_rect_frag.glsl");

    ShaderManager::getInstance()->loadAssetShader("primitive_rect" , 
        "shader/primitive_vert.glsl", "shader/primitive_rect_frag.glsl");
}

void RenderEngine::resetNormalMat(float w , float h){
    Logi(TAG , "reset normal matrix %f , %f" , w , h);
    normalMatrix_[0][0] = 2.0f / w;
    normalMatrix_[0][1] = 0.0f;
    normalMatrix_[0][2] = 0.0f;

    normalMatrix_[1][0] = 0.0f;
    normalMatrix_[1][1] = 2.0f / h;
    normalMatrix_[1][2] = 0.0f;

    normalMatrix_[2][0] = -1.0f;
    normalMatrix_[2][1] = -1.0f;
    normalMatrix_[2][2] =  1.0f;
}

void RenderEngine::submitRenderCommand(std::shared_ptr<RenderCommand> cmd){
    renderCommandList_.push_back(cmd);
}

//绘制自定义shader
void RenderEngine::renderShader(Shader &shader , 
                                Rect &showRect , 
                                std::function<void(void)> preRenderCallback){
    auto cmd = fetchShaderRenderCommand(this);
    cmd->putParams(shader ,showRect);
    cmd->setPreRenderCallback(preRenderCallback);

    cmd->runCommands();
    // submitRenderCommand(cmd);
}

void RenderEngine::renderText(std::wstring &text , 
        float left , float bottom , TextPaint &paint){
    // auto cmd = fetchTextRenderCommand(this);
    TextRenderCommand cmd(this);
    cmd.putParams(text , left , bottom , paint);
    cmd.runCommands();
    // submitRenderCommand(cmd);
}

void RenderEngine::renderTextWithRect(std::wstring &text , Rect &showRect , 
        TextPaint &paint, Rect *wrapContentRect){
    // auto cmd = fetchTextRenderCommand(this);
    TextRenderCommand cmd(this);
    cmd.limitRect_ = showRect;

    cmd.putTextParamsByRectLimit(text , showRect ,wrapContentRect , paint);
    cmd.runCommands();
    
    // submitRenderCommand(cmd);
}

std::shared_ptr<TextRenderCommand> RenderEngine::fetchTextRenderCommand(RenderEngine *engine){
    // new a new instace 
    // later use pool to reuse
    auto newCmd = 
        std::make_shared<TextRenderCommand>(this);
    newCmd->used = true;
    return newCmd;
}

std::shared_ptr<ShapeBatch> RenderEngine::getShapeBatch(){
    return shapeBatch_;
}

std::shared_ptr<ShaderRenderCommand> RenderEngine::fetchShaderRenderCommand(RenderEngine *engine){
    auto newCmd = 
        std::make_shared<ShaderRenderCommand>(this);
    newCmd->used = true;
    return newCmd;
}

void TextRenderHelper::loadRes(RenderEngine &engine){
    textRenderShader_ = ShaderManager::getInstance()->loadAssetShader("text_render" 
                                , "shader/render_text_vert.glsl"
                                , "shader/render_text_frag.glsl");
    
    buildTextCharConfig();
}

std::shared_ptr<CharInfo> TextRenderHelper::findCharInfo(wchar_t &ch){
    auto result = charInfoMaps_[ch];
    if(result == nullptr){
        // Logi("debug" , "in charinfos  %d not found" , ch);
        result = charInfoMaps_[L'*'];
    }
    return result;
}

//读取字符配置
void TextRenderHelper::buildTextCharConfig(){
    
    std::string fontFoldName = "font1/";

    std::wstring charConfigStr = AssetManager::getInstance()->readTextFile("text/"+ fontFoldName +"char_config.json");

    JsonParser parser;
    auto configJson = parser.parseJsonObject(charConfigStr);
    auto charJsonList = configJson->getJsonArray("list");
    Logi("text_render" , "charJsonList size : %d" , charJsonList->size());
    
    //load texture
    auto textureFileList = configJson->getJsonArray("textureFiles");
    std::vector<std::string> filelist;
    for(int i = 0 ; i < textureFileList->size() ;i++){
        std::string filename = ToByteString(textureFileList->getString(i));
        Logi("debug" , "filename: %s" , filename.c_str());
        filelist.push_back("text/" + fontFoldName +filename);
    }//end for i
    // auto textureInfo = TextureManager::getInstance()->acquireTexture("text/" + textureName);
    auto textureInfo = TextureManager::getInstance()->loadTextureArray(filelist);
    Logi("debug" , "load texture %d" , glGetError());
    if(textureInfo != nullptr){
        mainTextureId_ = textureInfo->textureId;
    }else{
        Logi("TextRenderHelper" , "error to load textureInfo");
        return;
    }

    Logi("TextRenderHelper" , "load texture name : %s , width : %d , height : %d" , 
        textureInfo->name.c_str(),
        textureInfo->width,
        textureInfo->height);

    charInfoMaps_.clear();
    for(int i = 0 ;  i< charJsonList->size();i++){
        auto itemJson = charJsonList->getJsonObject(i);
        std::shared_ptr<CharInfo> info = std::make_shared<CharInfo>();
        info->value = itemJson->getString("value");
        info->width = itemJson->getInt("width");
        info->height = itemJson->getInt("height");
        info->bearingX = itemJson->getInt("bearingX");
        info->bearingY = itemJson->getInt("bearingY");
        auto texCoordsArray = itemJson->getJsonArray("texCoords");
        for(int i = 0 ; i < texCoordsArray->size(); i++){
            info->textureCoords[i] = texCoordsArray->getFloat(i);
        }//end for i

        //load texture
        // auto textureName = itemJson->getString("texture");
        // std::string textureName = ToByteString(itemJson->getString("texture"));
        // Logi("text_render" , "textureName coord: %f , %f , %f , %f" , 
        //     info->textureCoords[0] ,info->textureCoords[1],
        //     info->textureCoords[2] ,info->textureCoords[3] );
        // Logi("text_render" , "texture value size %d" , info->value.size());
        // Logi("text_render" , "texture name  %s" , textureName.c_str());
        // mainTextureId_ = info->textureId;
        // // Logi("text_render" , "textureId : %d" , info->textureId);
        charInfoMaps_.insert(std::make_pair<>(info->value[0] , info));
    }//end for i

    Logi("text_render" , "charInfoMaps size : %d" , charInfoMaps_.size());
    Logi("text_render" , "texture : %s" , 
            TextureManager::getInstance()->allTextureInfos().c_str());

    addSpecialTextCharInfo();
}

//添加特殊字符配置
void TextRenderHelper::addSpecialTextCharInfo(){
    auto blankCharInfo = std::make_shared<CharInfo>();
    blankCharInfo->textureId = 0;
    blankCharInfo->value = L" ";
    blankCharInfo->width = SPACE_WIDTH;
    blankCharInfo->height = 0.0f;
    blankCharInfo->textureCoords[0] = 0.0f;
    blankCharInfo->textureCoords[1] = 0.0f;
    blankCharInfo->textureCoords[2] = 0.0f;
    blankCharInfo->textureCoords[3] = 0.0f;
    charInfoMaps_.insert(std::make_pair<>(blankCharInfo->value[0] , blankCharInfo));

    auto tabCharInfo = std::make_shared<CharInfo>();
    tabCharInfo->textureId = 0;
    tabCharInfo->value = L"\t";
    tabCharInfo->width = 4*SPACE_WIDTH;
    tabCharInfo->height = 0.0f;
    tabCharInfo->textureCoords[0] = 0.0f;
    tabCharInfo->textureCoords[1] = 0.0f;
    tabCharInfo->textureCoords[2] = 0.0f;
    tabCharInfo->textureCoords[3] = 0.0f;
    charInfoMaps_.insert(std::make_pair<>(tabCharInfo->value[0] , tabCharInfo));

    auto rCharInfo = std::make_shared<CharInfo>();
    rCharInfo->textureId = 0;
    rCharInfo->value = L"\r";
    rCharInfo->width = 0.0f;
    rCharInfo->height = 0.0f;
    rCharInfo->textureCoords[0] = 0.0f;
    rCharInfo->textureCoords[1] = 0.0f;
    rCharInfo->textureCoords[2] = 0.0f;
    rCharInfo->textureCoords[3] = 0.0f;
    charInfoMaps_.insert(std::make_pair<>(rCharInfo->value[0] , rCharInfo));

    auto nCharInfo = std::make_shared<CharInfo>();
    nCharInfo->textureId = 0;
    nCharInfo->value = L"\n";
    nCharInfo->width = 0.0f;
    nCharInfo->height = 0.0f;
    nCharInfo->textureCoords[0] = 0.0f;
    nCharInfo->textureCoords[1] = 0.0f;
    nCharInfo->textureCoords[2] = 0.0f;
    nCharInfo->textureCoords[3] = 0.0f;
    charInfoMaps_.insert(std::make_pair<>(nCharInfo->value[0] , nCharInfo));
}

void TextRenderHelper::loadSymbolMap(){
    std::wstring symbolStr = L"1234567890!@#$%^&*()_[]{}:;,.<>/?abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ，";
    for(wchar_t ch : symbolStr){
        SymbolMap.insert({ch , ch});
    }
    Logi("isymbol" , "map size = %d " , SymbolMap.size());
}

bool TextRenderHelper::isSymbol(std::wstring &ch){
    return SymbolMap.find(ch[0]) != SymbolMap.end();
}

std::shared_ptr<TextRenderCommand> RenderCommandCache::acquireTextRender(std::wstring &content , Rect &rect ,TextPaint &paint){
    return nullptr;
}

std::shared_ptr<ShapeRenderCommand> RenderEngine::fetchShaderShapeRenderCommand(RenderEngine *engine){
    auto newCmd = 
        std::make_shared<ShapeRenderCommand>(this);
    newCmd->used = true;
    return newCmd;
}


////绘制圆形
//void RenderEngine::renderCircle(float cx , float cy , float radius , Paint &paint){
//    Rect rect;
//    rect.left = cx - radius;
//    rect.top = cy + radius;
//    rect.width = 2 * radius;
//    rect.height = 2 * radius;
//
//    long long t1 = currentTimeMicro();
//    auto cmd = fetchShaderShapeRenderCommand(this);
//    cmd->putParams(rect , paint, ShapeType::ShapeCircle);
//    long long t2 = currentTimeMicro();
//     // Logi("renderCircle" , "put params time: %lld", (t2 - t1));
//    submitRenderCommand(cmd);
//}

////绘制矩形
//void RenderEngine::renderRect(Rect &rectangle ,Paint &paint){
//    auto cmd = fetchShaderShapeRenderCommand(this);
//    cmd->putParams(rectangle , paint, ShapeType::ShapeRect);
//    submitRenderCommand(cmd);
//}

//精灵类批量渲染
std::shared_ptr<SpriteBatch> RenderEngine::getSpriteBatch(){
    return spriteBatch_;
}

float RenderEngine::getAndChangeDepthValue(){
    float result = depthValue;
    depthValue -= 0.001f;
    return result;    
}

void RenderEngine::resetDepth(){
    depthValue = 1.0f;
}

/**
 * @brief  text layout calculate
 * 
 * @param content 
 * @param renderCmd 
 * @param outRect 
 * @param buf 
 */
void TextRenderHelper::layoutText(std::wstring &content, 
        TextRenderCommand *renderCmd,
        Rect &outRect,
        std::vector<float> &buf){
    
    TextPaint paint = renderCmd->paint_;
    Rect limitRect = renderCmd->limitRect_;
    
    outRect.left = limitRect.left;
    outRect.top = limitRect.top;
    outRect.width = 0.0f;
    outRect.height =(FONT_DEFAULT_SIZE + paint.gapSize) * paint.textSizeScale;

    float maxBaselineY = 0.0f;
    float lineWidth = 0.0f;
    bool isFirstLine = true;

    float x = limitRect.left;
    float y = limitRect.top;
    float depthValue = renderCmd->engine_->getAndChangeDepthValue();

    int index = 0;
    int realRenderCharCount = 0;
    const int size = content.length();
    while(index < size){
        wchar_t ch = content[index];

        auto charInfoPtr = findCharInfo(ch);

        if(isFirstLine){
            float bearingY = charInfoPtr->bearingY * paint.textSizeScale;
            if(maxBaselineY <= bearingY){
                maxBaselineY = bearingY;
            }
        }

        float charRealWidth = (charInfoPtr->width + paint.gapSize) * paint.textSizeScale;

        if(x + charRealWidth <= limitRect.getRight() && ch != L'\n'){
            renderCmd->putVertexDataToBuf(buf , index , x , y ,depthValue,
                charInfoPtr , paint);
            
            x += charRealWidth;
            lineWidth += charRealWidth;
            if(outRect.width < lineWidth){
                outRect.width = lineWidth;
            }
            index++;
            realRenderCharCount = index;
        }else{// change a new line
            isFirstLine = false;

            x = limitRect.left;
            y -= (FONT_DEFAULT_SIZE + paint.gapSize) * paint.textSizeScale;
            if(y - maxBaselineY < limitRect.getBottom()){
                break;
            }

            outRect.height += (FONT_DEFAULT_SIZE + paint.gapSize) * paint.textSizeScale;
            lineWidth = 0.0f;

            if(ch == L'\n'){
                index++;
            }
        }
    }//end while

    // if(realRenderCharCount < content.size()){
    //     buf.resize(realRenderCharCount * renderCmd->vertCountPerChar_);
    // }

    float translateX = limitRect.left - outRect.left;
    float translateY = -maxBaselineY;

    switch(paint.textGravity){
        case TopLeft:
            break;
        case TopCenter:
            translateX += (limitRect.width / 2.0f - outRect.width / 2.0f);
            outRect.left += translateX;
            break;
        case TopRight:
            translateX += (limitRect.width - outRect.width);
            outRect.left += translateX;
            break;
        case BottomLeft:
            translateY -= (limitRect.height - outRect.height);
            outRect.top += translateY + maxBaselineY;
            break;
        case BottomCenter:
            translateX += (limitRect.width / 2.0f - outRect.width / 2.0f);
            outRect.left += translateX;
            translateY -= (limitRect.height - outRect.height);
            outRect.top += translateY + maxBaselineY;
            break;
        case BottomRight:
            translateX += (limitRect.width - outRect.width);
            outRect.left += translateX;
            translateY -= (limitRect.height - outRect.height);
            outRect.top += translateY + maxBaselineY;
            break;
        case CenterLeft:
            translateY -= (limitRect.height / 2.0f - outRect.height / 2.0f);
            outRect.top += translateY + maxBaselineY;
            break;
        case CenterRight:
            translateX += (limitRect.width - outRect.width);
            outRect.left += translateX;
            translateY -= (limitRect.height / 2.0f - outRect.height / 2.0f);
            outRect.top += translateY + maxBaselineY;
            break;
        case Center:
            translateX += (limitRect.width / 2.0f - outRect.width / 2.0f);
            outRect.left += translateX;
            translateY -= (limitRect.height / 2.0f - outRect.height / 2.0f);
            outRect.top += translateY + maxBaselineY;
            break;
    }//end switch

    for(int i = 0 ; i < realRenderCharCount ;i++){
        renderCmd->updateVertexPositionData(buf , i , translateX , translateY);
    }//end for i
}

void RenderEngine::renderRect(Rect &rect , glm::mat4 &transMat , 
        Paint &paint){
    RectRenderCommand cmd(this);
    Shader rectShader = ShaderManager::getInstance()->getShaderByName("primitive_rect");
    cmd.putParams(rectShader , rect ,transMat, paint);
    cmd.runCommands();
}
