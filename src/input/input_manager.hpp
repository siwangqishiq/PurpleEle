//
// 用户输入
//
#pragma once

const int ACTION_DOWN = 1;
const int ACTION_MOVE = 2;
const int ACTION_UP = 3;

class InputAction{
public:
    virtual void onAction() = 0;
};

class InputManager{
public:
    InputManager();
    
private:
    bool isKeyBoardPressed = false;
};

