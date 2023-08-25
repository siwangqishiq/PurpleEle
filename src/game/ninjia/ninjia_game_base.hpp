#pragma once

class NinjiaGame;

class GameBaseObject{
public:
    GameBaseObject(NinjiaGame *game){
        gameContext_ = game;
    }
    
    NinjiaGame *gameContext_;
};