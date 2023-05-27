#ifndef LAB5GAMEOFLIFEPARALLEL_CLASS_GAME_OF_LIFE_H
#define LAB5GAMEOFLIFEPARALLEL_CLASS_GAME_OF_LIFE_H

#include "class_universe.h"
#include "class_game_data_scheduler.h"

class GameOfLife {
public:
    GameOfLife(int, int);

    void CopyCurrentUniverse();
    void GetNextCellGeneration();
    void ChangeCellState(Cell&);
    bool IsAnyMatchUniverse() const;

    void ShowLastUniverse() const;
    void StartNewGame();
    void PrepareGameResource();
    void EndGame();
private:
    bool IsEqualUniverse(const Universe &u) const;
    void UpdateUniversePool();

    bool is_loop_;
    int it_cnt_;

    GameDataScheduler scheduler_;
    Universe universe_part_;
    std::vector<Universe> universe_pool_;
};

#endif
