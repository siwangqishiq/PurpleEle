#include <iostream>
#include <string>
#include "../libjson/json.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H  

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const float FONT_DEFAULT_SIZE = 64.0f;


void blit(unsigned char *src , uint32_t *dst , int x , int y , int width , int height ,
            int strideWidth){
    for(int i = 0 ; i < height ;i++){
        for(int j = 0 ; j < width ;j++){
            if(src[i * width + j] == 0){
                dst[(y + i) * strideWidth + x + j] = 0x00000000;
            }else{
                //abgr
                dst[(y + i) * strideWidth + x + j] = 0xffffffff;
            }
        }//end for j
    }
}

/**
 * @brief 
 * 
 * width	face->glyph->bitmap.width	位图宽度（像素）
 * height	face->glyph->bitmap.rows	位图高度（像素）
 * bearingX	face->glyph->bitmap_left	水平距离，即位图相对于原点的水平位置（像素）
 * bearingY	face->glyph->bitmap_top	垂直距离，即位图相对于基准线的垂直位置（像素）
 * advance	face->glyph->advance.x	水平预留值，即原点到下一个字形原点的水平距离（单位：1/64像素）
 * 
 * 
 * @return int 
 */
int exportFonts(){
    std::string fontName = "yahei";
    
    FT_Library ftLib;

    if (FT_Init_FreeType(&ftLib)){
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    std::string fontPath = fontName + ".ttc";
    if (FT_New_Face(ftLib, fontPath.c_str(), 0, &face)){
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }

    const int fontSize = FONT_DEFAULT_SIZE;

    FT_Set_Pixel_Sizes(face , fontSize, fontSize);

    auto outputJson = JsonObject::create();
    auto charListArray = JsonArray::create();
    outputJson->putJsonArray("list" , charListArray);

    std::wstring content = ReadTextFileAsWstring("chars.txt");
    // std::wstring content = L"abc";
    std::cout << "char file size : " << content.length() << std::endl;

    const int outTexWidth = 2048;
    const int outTexHeight = 4 * 2048;

    uint32_t *textureDst = new uint32_t[outTexWidth * outTexHeight];

    int left = 0;
    int top = 0;
    int progress = 0;

    int currentFileIndex = 0;
    std::string currentTexFileName = "font_texture_"+std::to_string(currentFileIndex)+".png";

    for(wchar_t ch : content){
        progress++;
        std::cout << progress << " / " << content.length() << std::endl;
        if(ch == L' '
            || ch == L'\t'  
            || ch == L'\n' 
            || ch == L'\r'){
            continue;
        }

        // std::wcout << "left " << left << "  top " << top << std::endl;

        if (FT_Load_Char(face, ch, FT_LOAD_RENDER)){
            std::wcout<< ch << " ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        
        // std::cout << "width = " << face->glyph->bitmap.width << std::endl;
        // std::cout << "height = " << face->glyph->bitmap.rows << std::endl;
        // std::cout << "pitch  = " << face->glyph->bitmap.pitch << std::endl;

        auto glyph = face->glyph;
        int fontWidth = static_cast<int>(face->glyph->bitmap.width);
        int fontHeight = static_cast<int>(face->glyph->bitmap.rows); // std::min(static_cast<int>(face->glyph->bitmap.rows) , fontSize);
        FT_Bitmap fontBit = glyph->bitmap;

        int topOffset = fontSize - fontHeight;
        // std::cout << "top offset " << topOffset << std::endl;
        blit(glyph->bitmap.buffer , 
            textureDst , 
            left ,
            top + topOffset , 
            fontWidth , 
            fontHeight , outTexWidth);

        auto charInfoJson = JsonObject::create();
        std::wstring value = L"";
        value += ch;
        charInfoJson->putString("value" , value);
        charInfoJson->putInt("width" , fontWidth);
        charInfoJson->putInt("height" , fontHeight);
        charInfoJson->putString("texture" , ToWideString(currentTexFileName));
        //纹理坐标计算
        float texLeft = (float)left / (float)outTexWidth;
        float texTop = (float)(top + (fontSize - fontHeight)) / (float)outTexHeight;
        float texWidth = (float)fontWidth / (float)outTexWidth;
        float texHeight = (float)fontHeight / (float)outTexHeight;

        if(texLeft >= 1.0){
            std::wcout << "left " << left << "  top " << top << std::endl;
        }

        auto texCoords = JsonArray::create();
        texCoords->pushFloat(texLeft);
        texCoords->pushFloat(texTop);
        texCoords->pushFloat(texLeft + texWidth);
        texCoords->pushFloat(texTop + texHeight);
        charInfoJson->putJsonArray("texCoords" , texCoords);
        
        charListArray->pushJsonObject(charInfoJson);

        if(left + fontSize >= outTexWidth){
            left = 0;
            if(top + fontSize > outTexHeight - fontSize){
                std::cout << "need create a new texture file" << std::endl;
                stbi_write_png(currentTexFileName.c_str() , outTexWidth, outTexHeight, 4 , textureDst , 0);
                delete []textureDst;
                top = 0;

                textureDst = new uint32_t[outTexWidth * outTexHeight];
                currentFileIndex++;
                currentTexFileName = "font_texture_"+std::to_string(currentFileIndex)+".png";
                std::cout << "create new texture file " << currentFileIndex << std::endl;
            }else{
                top += fontSize;
            }
        }else{
            left += fontSize;
        }
    }//end for each

    stbi_write_png(currentTexFileName.c_str() , outTexWidth, outTexHeight, 4 , textureDst , 0);
    delete []textureDst;

    WriteStringToFile("char_config.json", outputJson->toJsonString());

    JsonParser parser;
    auto str = outputJson->toJsonString();
    auto result = parser.parseJsonObject(str);
    auto charList = result->getJsonArray("list");

    std::wcout << "total size " << charListArray->size() << std::endl;

    return 0;
}

int printChar(FT_Library *ftLib , wchar_t wChar){
    FT_Face face;
    if (FT_New_Face(*ftLib, "songti.ttc", 0, &face)){
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    
    FT_Set_Pixel_Sizes(face , 0 , 32);

    if (FT_Load_Char(face, wChar, FT_LOAD_RENDER)){
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        return -1;
    }

    std::cout << "width = " << face->glyph->bitmap.width << std::endl;
    std::cout << "height = " << face->glyph->bitmap.rows << std::endl;
    std::cout << "pitch  = " << face->glyph->bitmap.pitch << std::endl;
    std::cout << "left = " << face->glyph->bitmap_left << std::endl;
    std::cout << "top  = " << face->glyph->bitmap_top << std::endl;
    
    std::cout << "advance  = " << face->glyph->advance.x / 64 << 
        " , " << face->glyph->advance.y << std::endl;

    FT_Bitmap fontBit = face->glyph->bitmap;

    for(int i = 0 ;  i < fontBit.rows ; i++ ){
        for(int j = 0 ; j < fontBit.width ; j++){
            std::cout << ((fontBit.buffer[i * fontBit.pitch + j] == 0)?" ":"+");
        }
        std::cout<< std::endl;
    }

    FT_Done_Face(face);

    return 0;
}

int main(){
    exportFonts();
    return 0;
}