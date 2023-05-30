#pragma once
#include "mpi.h"

#include "../game-object/game_field.h"

class GameOfLife{
public:
    GameOfLife() = default;
    void InitGameResource(int ,int);
    void StartGame(int, int);
    void EndGame();
private:
    void GameProcess();
    void UpdateHistory();
    bool SpawnNewGeneration();
    bool IterateThroughHistory() const;

    int iter_cnt_;
    double work_time_;
    GameField buffer_field_;
    std::vector<GameField> history_;
};