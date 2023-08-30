#pragma once

class TextureImage;
class TextureImageRegion;

class SkyBackground : public GameBaseObject{
public:
    SkyBackground(NinjiaGame *game):GameBaseObject(game){
    }

    void init();

    void update();

    void render();

    void renderByCamera(Camera &cam);

    void dispose();
private:
    std::shared_ptr<TextureImage> skyBackgroundImage_;
    float offset_;

    const float SCROLL_SPEED_RATIO = 0.05f;
};

enum ObstraceState{
    Normal,
    Hitted
};

struct Obstrace{
    int type;
    glm::vec2 position;
    glm::vec2 size;
    ObstraceState state = Normal;
    Rect hitRect;
};

const int TERRAIN_TILE_TYPE_FOREST = 1;
const int TERRAIN_TILE_TYPE_NOFOREST = 2;


struct TerrainTile{
    int type;
    std::shared_ptr<std::vector<Obstrace>> obstraces;
};

class Terrain : public GameBaseObject{
public:
    Terrain(NinjiaGame *game):GameBaseObject(game){
    }

    void init();

    void update(Camera &cam);

    void renderByCamera(Camera &cam);
    
    void render();

    void dispose();

    bool collisionDetect();

    float terrainHeight_;
    float forestBgHeight_;
    float forestTop_;

    // float firstTileX_;
    // float secondTileX_;

    float lastPlayerPosX_ = 0.0f;

    bool showForest = true;
private:
    std::shared_ptr<TextureImage> terrainImage_;
    std::shared_ptr<TextureImage> forestImage_;
    std::shared_ptr<TextureImage> storesImage_;

    std::vector<std::shared_ptr<TextureImageRegion>> storeImageList_;

    float posX_ = 0.0f;

    void initTerrainMapData();

    void addObstraceToTile(TerrainTile &tile , int index);

    std::vector<TerrainTile> tileData_;

    std::vector<int> renderTileList_;
};
