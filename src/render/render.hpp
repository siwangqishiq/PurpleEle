/**
 * @file render.hpp
 * @author panyi
 * @brief 
 * @date 2022-11-17
 * 
 * renderer
 * 
 * 
 */
#pragma once
#include <memory>
#include "glheader.hpp"
#include <string>
#include <vector>
#include "glm/matrix.hpp"
#include "render/shader.hpp"
#include <unordered_map>
#include "common.hpp"
#include "log.hpp"
#include <functional>


class Application;
class RenderCommand;
class TextRenderHelper;
class TextRenderCommand;
class ShaderRenderCommand;
class ShapeRenderCommand;
class VRamManager;

class ShapeBatch;
class SpriteBatch;

class RenderEngine{
public:
    const std::string TAG = "RenderEngine";

    RenderEngine(Application *appContext) : appContext_(appContext){
        Logi("RenderEngine" , "Render Engine con");
    }

    ~RenderEngine(){
        Logi("RenderEngine" , "~RenderEngine Engine deconstruct");
    }

    Application *appContext_;

    void init();

    void clearRenderCommands();

    void submitRenderCommand(std::shared_ptr<RenderCommand> cmd);

    void render();

    void free();

    void onScreenResize();

    std::shared_ptr<TextRenderHelper> textRenderHelper_;

    //归一化变换矩阵
    glm::mat3 normalMatrix_;

    //==============render api ================
    //弧线
    void renderArc(float cx , float cy , float radius , 
            float beginAngle , float endAngle , Paint &paint);

    //画单色直线
    void renderLines(std::vector<float> &points , Paint &paint);

    //绘制单独的一个矩形
    void renderRect(Rect &rect , glm::mat4 &transMat , Paint &paint);

    //绘制文字
    void renderText(std::wstring &text , float left , float bottom , TextPaint &paint);

    void renderText(const wchar_t *text , float left , float bottom , TextPaint &paint){
        auto str = std::wstring(text);
        renderText(str, left , bottom , paint);
    }

    //绘制文字  但是将文字限定在一个矩形框内 放不下的文字 直接舍弃 
    void renderTextWithRect(std::wstring &text , Rect &showRect , 
            TextPaint &paint , 
            Rect *wrapContentRect);//

    void renderTextWithRect(const wchar_t *text , 
            Rect &showRect , 
            TextPaint &paint ,
            Rect *wrapContentRect){
        auto str = std::wstring(text);
        renderTextWithRect(str , showRect , paint ,wrapContentRect);
    }

    //在指定矩形区域内绘制自定义shader
    void renderShader(Shader &shader , Rect &showRect , 
            std::function<void(void)> preRenderCallback);

    std::shared_ptr<ShapeBatch> getShapeBatch();

    std::shared_ptr<SpriteBatch> getSpriteBatch();

    float getAndChangeDepthValue();
    
    void resetDepth();

    std::shared_ptr<VRamManager> vramManager_;
private:
    std::vector<std::shared_ptr<RenderCommand>> renderCommandList_;

    void loadTextRenderResource();
    
    void loadShapeShader();

    void resetNormalMat(float w , float h);

    int textCommandIndex;
    std::vector<std::shared_ptr<TextRenderCommand>> textCommandPool;

    //fetch new render command
    std::shared_ptr<TextRenderCommand> fetchTextRenderCommand(RenderEngine *engine);
    std::shared_ptr<ShaderRenderCommand> fetchShaderRenderCommand(RenderEngine *engine);
    std::shared_ptr<ShapeRenderCommand> fetchShaderShapeRenderCommand(RenderEngine *engine);


    //batch render
    std::shared_ptr<ShapeBatch> shapeBatch_;

    std::shared_ptr<SpriteBatch> spriteBatch_;

    float depthValue = 1.0f;

    int viewWidth_;
    int viewHeight_;
};

//字符信息
struct CharInfo{
    std::wstring value; 
    float width;
    float height;
    float bearingX;
    float bearingY;
    float textureCoords[5];
    unsigned int textureId;
};

//字符默认高度
static const float CHAR_DEFAULT_HEIGHT = 64.0f;
static const float SPACE_WIDTH = 16.0f;

class TextRenderHelper{
public:
    void loadRes(RenderEngine &engine);

    Shader textRenderShader_;

    std::shared_ptr<CharInfo> findCharInfo(wchar_t &ch);

    void layoutText(std::wstring &content , 
        TextRenderCommand *renderCmd,
        Rect &outRect,
        std::vector<float> &buf);

    unsigned int mainTextureId_;

    static void loadSymbolMap();

    static bool isSymbol(std::wstring &ch);

private:
    void buildTextCharConfig();

    void addSpecialTextCharInfo();

    std::unordered_map<wchar_t , std::shared_ptr<CharInfo>> charInfoMaps_;
};

//渲染命令缓存
class RenderCommandCache{
public:
    std::shared_ptr<TextRenderCommand> acquireTextRender(std::wstring &content , Rect &rect ,TextPaint &paint);
};

