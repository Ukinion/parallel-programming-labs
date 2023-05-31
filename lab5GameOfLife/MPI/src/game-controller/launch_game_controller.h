#pragma once

#include <memory>

#include "../game-model/game_of_life.h"

class LaunchController {
public:
    LaunchController();
    void SetupGameSettings(int, int);
    void PrepareGameResources();
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
    class GameResource {
    public:
        GameField part_field;
    };
    void CountDataForScatter(int*, int*, int*);

    std::unique_ptr<GameOfLife> game_;
    GameSetting settings_;
    GameResource resources_;
};