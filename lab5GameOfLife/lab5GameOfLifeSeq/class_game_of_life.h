#ifndef LAB5GAMEOFLIFE_CLASS_GAME_OF_LIFE_H
#define LAB5GAMEOFLIFE_CLASS_GAME_OF_LIFE_H

#include "class_universe.h"

class GameOfLife
{
public:
    GameOfLife(int, int);

    void CopyCurrentUniverse();
    void GetNextCellGeneration();
    void ChangeCellState(Cell&);
    bool IsAnyMatchUniverse() const;

    void ShowLastUniverse() const;
    void StartGame();
    void EndGame();
private:
    bool IsEqualUniverse(const Universe &u) const;
    void UpdateUniversePool();

    Universe support_;
    std::vector<Universe> universe_pool_;
};


#endif
