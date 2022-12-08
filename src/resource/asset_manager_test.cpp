#include "utest.hpp"
#include <iostream>
#include "asset_manager.hpp"

int main(){
    Test("test asset mgr read text" , [](){
        std::wstring content = AssetManager::getInstance()->readTextFile("test.txt");
        EqualWString(L"Desktop Hello World!你好世界" , content);
    });

    Test("test asset mgr read image" , [](){
        TextureFileConfig config;
        auto data = AssetManager::getInstance()->readTextureFile("text/font_texture_0.png" , config);
        
        Equal(2048 , config.width);
        Equal(2048 , config.height);
        Equal(4 , config.channel);
    });
    
    return utest.testAll();
}