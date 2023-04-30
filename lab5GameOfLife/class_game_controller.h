#ifndef LAB5GAMEOFLIFE_CLASS_GAME_CONTROLLER_H
#define LAB5GAMEOFLIFE_CLASS_GAME_CONTROLLER_H

#include <list>
#include "class_universe.h"

class GameController
{
public:
    GameController(int, int);

    void CopyCurrentUniverse();
    void ChangeCurrentUniverse();
    void GetNextCellGeneration(const Universe&);
    bool CompareCurUniverseWithPrev() const;

    void StartGame();
    void EndGame();
private:
    bool IsEqualUniverse() const;

    Universe support_;
    std::vector<Universe> universe_pool_;
    int pool_size_;
};


#endif
