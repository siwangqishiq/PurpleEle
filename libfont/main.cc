#include <iostream>
#include <string>
#include "../libjson/json.hpp"
#include <algorithm>

#include "ft2build.h"
#include FT_FREETYPE_H  

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct CharData{
    int fontWidth;
    int fontHeight;
    int outWidth;
    int outHeight;
    int offsetX;
    int offsetY;
    int bearingX;
    int bearingY;
    uint8_t *pData;
};

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

void blit2(uint8_t *src , uint8_t *dst , int x , int y , int width , int height ,
            int strideWidth , int dstHeight){
    for(int i = 0 ; i < height ;i++){
        for(int j = 0 ; j < width ;j++){
            auto offset = (y + i) * strideWidth + x + j;
            // if(offset >= strideWidth * dstHeight){
            //     continue;
            // }

            if(src[i * width + j] == 0){
                dst[offset] = 0x00;
            }else{
                //abgr
                dst[offset] = 0xff;
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

    FT_Set_Pixel_Sizes(face , 0, fontSize);

    auto outputJson = JsonObject::create();
    auto charListArray = JsonArray::create();
    outputJson->putJsonArray("list" , charListArray);

    std::wstring content = ReadTextFileAsWstring("all_char.txt");
    // std::wstring content = L"abc";
    std::cout << "char file size : " << content.length() << std::endl;

    const int outTexWidth = 8 * 1024;
    const int outTexHeight = 16 * 1024;

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
        charInfoJson->putInt("offset_x" , glyph->bitmap_left);
        charInfoJson->putInt("offset_y" , glyph->bitmap_top);
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

int buildWcharTexture(wchar_t wChar , FT_Face &face , int fontHeight , CharData &charData){
    if (FT_Load_Char(face, wChar, FT_LOAD_RENDER)){
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        return -1;
    }

    FT_Bitmap fontBit = face->glyph->bitmap;

    charData.fontWidth = fontBit.pitch;
    charData.fontHeight = fontBit.rows;

    charData.outWidth = std::max(static_cast<int>(face->glyph->advance.x / 64) , 
        charData.fontWidth);
    charData.outHeight = fontHeight;

    charData.bearingX = face->glyph->bitmap_left;
    
    charData.bearingY = face->glyph->bitmap_top;

    charData.pData = new uint8_t[charData.outWidth * charData.outHeight];
    
    //init;
    for(int i = charData.outHeight - 1 ; i >= 0; i--){
        for(int j = 0; j < charData.outWidth;j++){
            charData.pData[charData.outWidth * i + j] = 0;
        }//end for j
    }//end for i

    int offset_x = std::max(0 , (charData.outWidth - charData.fontWidth) / 2);
    int offset_y = 0;

    charData.offsetX = offset_x;
    charData.offsetY = offset_y;

    // std::cout << "outWidth   :" << charData.outWidth << std::endl;
    // std::cout << "outHeight  :" << charData.outHeight << std::endl;
    // std::cout << "fontWidth  :" << charData.fontWidth << std::endl;
    // std::cout << "fontHeight :" << charData.fontHeight << std::endl;
    // std::cout << "offsetx    :" << charData.offsetX << std::endl;
    // std::cout << "offsety    :" << charData.offsetY << std::endl;
    // std::cout << "bearingX   :" << charData.bearingX << std::endl;
    // std::cout << "bearingY   :" << charData.bearingY << std::endl;

    for(int i = 0 ;  i < fontBit.rows ; i++ ){
        for(int j = 0 ; j < fontBit.pitch ; j++){
            if(fontBit.buffer[i * fontBit.pitch + j] != 0){
                if((i + offset_y) * charData.outWidth + j + offset_x < charData.outWidth * charData.outHeight){
                    charData.pData[(i + offset_y) * charData.outWidth + j + offset_x] = 1;
                }
            }
        }
    }

    return 0;
}

int printChar(wchar_t wChar){
    FT_Library ftLib;

    if (FT_Init_FreeType(&ftLib)){
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }//

    FT_Face face;
    if (FT_New_Face(ftLib, "yahei.ttc", 0, &face)){
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    
    const int fontHeight = 16;

    FT_Set_Pixel_Sizes(face , 0 , fontHeight);

    if (FT_Load_Char(face, wChar, FT_LOAD_RENDER)){
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        return -1;
    }

    std::cout << "width = " << face->glyph->bitmap.width << std::endl;
    std::cout << "height = " << face->glyph->bitmap.rows << std::endl;
    std::cout << "advance  = " << (face->glyph->advance.x / 64) << 
            " , " << (face->glyph->advance.y / 64)  << std::endl;
    std::cout << "pitch  = " << face->glyph->bitmap.pitch << std::endl;
    std::cout << "bearingX = " << face->glyph->bitmap_left << std::endl;
    std::cout << "bearingY = " << face->glyph->bitmap_top << std::endl;

    int outputWidth = face->glyph->advance.x / 64;
    int outputHeight = fontHeight;

    std::cout << "outWidth = " << outputWidth << std::endl;
    std::cout << "outHeight = " << outputHeight << std::endl;

    auto metrics = face->glyph->metrics;
    float ratio = static_cast<float>(face->glyph->bitmap.width) / metrics.width;
    int baseLineY = ratio * metrics.horiBearingY;
    std::cout << "baseLineY = " << baseLineY << std::endl;

    int *outputBit = new int[outputWidth * outputHeight];
    //init;
    for(int i = outputHeight - 1 ; i >= 0; i--){
        for(int j = 0; j < outputWidth;j++){
            outputBit[outputWidth * i + j] = 0;
        }//end for j
    }//end for i
    
    FT_Bitmap fontBit = face->glyph->bitmap;

    int offset_x = (outputWidth - fontBit.pitch) / 2.0;
    if(offset_x < 0){
        offset_x = 0;
    }

    int offset_y = outputHeight - face->glyph->bitmap_top;
    if(offset_y + face->glyph->bitmap.rows > outputHeight){
        offset_y = outputHeight - face->glyph->bitmap.rows;
    }

    std::cout << "offset_x : " << offset_x 
        << " offset_y : " << offset_y 
        << " " << offset_y + face->glyph->bitmap.rows <<std::endl;

    for(int i = 0 ;  i < fontBit.rows ; i++ ){
        for(int j = 0 ; j < fontBit.pitch ; j++){
            if(fontBit.buffer[i * fontBit.pitch + j] != 0){
                outputBit[(i + offset_y) * outputWidth + j + offset_x] = 1;
            }
        }
    }

    //print on console
    for(int i = 0 ; i < outputHeight;i++){
        for(int j = 0; j < outputWidth;j++){
            std::cout << outputBit[i * outputWidth + j];
        }
        std::cout<< std::endl;
    }

    FT_Done_Face(face);

    return 0;
}

void clearTextureData(uint8_t *data , int width , int height){
    for(int i = 0 ; i< height ;i++){
        for(int j = 0 ; j< width ;j++){
            data[i * width + j] = 0;
        }
    }
}

bool checkHasStr(JsonArray &list , std::string &str){
    for(int i = 0 ; i < list.size() ; i++){
        auto itemStr = ToByteString(list.getString(i));
        if(itemStr == str){
            return true;
        }
    }
    return false;
}

void saveImageToFile(std::string filename , 
        int outTexWidth , int outTexHeight , 
        uint8_t *outDst){
    stbi_write_png(filename.c_str() , outTexWidth, outTexHeight, 1 , outDst , 0);
    // stbi_write_jpg(filename.c_str() , outTexWidth, outTexHeight, 1 , outDst , 1);
}

int exportFonts2(){
    std::string fontName = "yahei";
    
    FT_Library ftLib;

    if (FT_Init_FreeType(&ftLib)){
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    std::string fontPath = fontName + ".ttc";
    fontPath = "shouxie.ttf";
    if (FT_New_Face(ftLib, fontPath.c_str(), 0, &face)){
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }

    const int fontSize = 128;
    const int fontRowGapSize = 16;

    FT_Set_Pixel_Sizes(face , 0, fontSize);

    auto outputJson = JsonObject::create();
    auto charListArray = JsonArray::create();
    outputJson->putJsonArray("list" , charListArray);

    std::wstring content = ReadTextFileAsWstring("chars.txt");
    std::cout << "char file size : " << content.length() << std::endl;

    const int outTexWidth = 2 * 1024;
    const int outTexHeight = 2 * 1024;

    float left = 0;
    float top = 0;
    int progress = 0;
    int currentFileIndex = 0;
    std::string currentTexFileName = "font_texture_"+std::to_string(currentFileIndex)+".png";
    uint8_t *textureDst = new uint8_t[outTexWidth * outTexHeight];
    clearTextureData(textureDst , outTexWidth , outTexHeight);

    auto textureFiles = JsonArray::create();
    textureFiles->pushString(ToWideString(currentTexFileName));
    outputJson->putJsonArray("textureFiles" , textureFiles);

    for(wchar_t ch : content){
        progress++;
        std::cout << progress << " / " << content.length() << std::endl;
        if(ch == L' ' || ch == L'\t' || ch == L'\n' || ch == L'\r'){
            continue;
        }

        if (FT_Load_Char(face, ch, FT_LOAD_RENDER)){
            std::wcout<< ch << " ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        CharData charData;
        if(buildWcharTexture(ch , face , fontSize , charData) < 0){
            std::wcout<< ch << " ERROR::FREETYTPE: Failed to load char data" << std::endl;
            continue;
        }

        // std::cout << "buildWcharTexture ended!" << std::endl;
        // print on console
        // for(int i = 0 ; i < charData.outHeight;i++){
        //     for(int j = 0; j < charData.outWidth;j++){
        //         if(charData.pData[i * charData.outWidth + j] != 0){
        //             std::cout << "*";
        //         }else{
        //             std::cout << "0";
        //         }
        //     }//end for j
        //     std::cout<< std::endl;
        // }//end for i

        std::cout << "left " << left << " top " << top 
            << " dstW = " << outTexWidth 
            << " dstH = " << outTexHeight <<  std::endl; 
        

        std::cout << "start blit2" << std::endl;
        blit2(charData.pData , textureDst , left , top, 
            charData.outWidth , charData.outHeight , outTexWidth , outTexHeight);

        //write meta json data
        auto jsonObject = JsonObject::create();
        std::wstring str = L"";
        str += ch;
        jsonObject->putString("value" , std::wstring(str));

        jsonObject->putInt("width" , charData.fontWidth);
        jsonObject->putInt("height" , charData.fontHeight);
        jsonObject->putInt("bearingX" , charData.bearingX);
        jsonObject->putInt("bearingY" , charData.bearingY);
        
        float texLeft = static_cast<float>(left + charData.offsetX) / static_cast<float>(outTexWidth);
        float texTop = static_cast<float>(top + charData.offsetY) / static_cast<float>(outTexHeight);
        float texWidth = static_cast<float>(charData.fontWidth) / static_cast<float>(outTexWidth);
        float texHeight = static_cast<float>(charData.fontHeight) / static_cast<float>(outTexHeight);

        auto texCoords = JsonArray::create();
        texCoords->pushFloat(texLeft);
        texCoords->pushFloat(texTop);
        texCoords->pushFloat(texLeft + texWidth);
        texCoords->pushFloat(texTop + texHeight);
        texCoords->pushFloat(static_cast<float>(currentFileIndex));
        jsonObject->putJsonArray("texCoords" , texCoords);

        jsonObject->putString("texture" , ToWideString(currentTexFileName));

        if(left + fontSize >= outTexWidth){
            left = 0.0;
            top += (fontSize + fontRowGapSize);

            if(top > outTexHeight - fontSize){//chang a new texture
                left = 0.0f;
                top = 0.0;

                if(!checkHasStr(*textureFiles , currentTexFileName)){
                    textureFiles->pushString(ToWideString(currentTexFileName));
                }

                saveImageToFile(currentTexFileName , 
                                outTexWidth , outTexHeight, 
                                textureDst);

                currentFileIndex++;
                currentTexFileName = "font_texture_"+std::to_string(currentFileIndex)+".png";
                clearTextureData(textureDst , outTexWidth , outTexHeight);
            }
        }else{
            left += fontSize;
        }

        delete charData.pData;

        charListArray->pushJsonObject(jsonObject);
    }//end for each

    FT_Done_Face(face);
    

    if(!checkHasStr(*textureFiles , currentTexFileName)){
        textureFiles->pushString(ToWideString(currentTexFileName));
    }
    saveImageToFile(currentTexFileName , outTexWidth , outTexHeight , textureDst);

    delete []textureDst;

    WriteStringToFile("char_config.json", outputJson->toJsonString());

    JsonParser parser;
    auto str = outputJson->toJsonString();
    auto result = parser.parseJsonObject(str);
    auto charList = result->getJsonArray("list");

    std::wcout << "total size " << charListArray->size() << std::endl;
    return 0;
}

int main(){
    // exportFonts();
    // std::wstring str = L"j";
    // for(auto &ch : str){
    //     printChar(ch);
    // }//end for 
    
    exportFonts2();

    return 0;
}