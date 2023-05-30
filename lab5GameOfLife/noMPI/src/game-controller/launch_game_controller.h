#pragma once

#include <memory>

#include "../game-model/game_of_life.h"

class LaunchController {
public:
    LaunchController();
    void SetupGameSettings(int, int);
    void LaunchGame();
    void WithdrawGame();
private:
    class GameSetting {
    public:
        int row;
        int col;
        int start_x;
        int start_y;
    };
    void RequestPatternStartPos();
    std::unique_ptr<GameOfLife> game_;
    GameSetting settings_;
};