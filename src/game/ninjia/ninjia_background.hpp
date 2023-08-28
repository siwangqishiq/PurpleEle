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

    void update();

    void renderByCamera(Camera &cam);
    
    void render();

    void dispose();

    float terrainHeight_;
    float forestBgHeight_;
    float forestTop_;

    float firstTileX_;
    float secondTileX_;

    float lastPlayerPosX_ = 0.0f;
private:
    std::shared_ptr<TextureImage> terrainImage_;
    std::shared_ptr<TextureImage> forestImage_;

    float posX_ = 0.0f;
};
