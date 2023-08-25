#pragma once

class TextureImage;

class SkyBackground : public GameBaseObject{
public:
    SkyBackground(NinjiaGame *game):GameBaseObject(game){
    }

    void init();

    void render();

    void dispose();
private:
    std::shared_ptr<TextureImage> skyBackgroundImage_;
};

class Terrain : public GameBaseObject{
public:
    Terrain(NinjiaGame *game):GameBaseObject(game){
    }

    void init();
    
    void render();

    void dispose();

    float terrainHeight_;
    float forestBgHeight_;
    float forestTop_;

private:
    std::shared_ptr<TextureImage> terrainImage_;
    std::shared_ptr<TextureImage> forestImage_;
};
