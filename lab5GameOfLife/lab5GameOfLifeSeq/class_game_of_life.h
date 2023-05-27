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
    void ShowTranslateTable() const;
    void StartNewGame();
    void EndGame();
private:
    bool IsEqualUniverse(const Universe &u) const;
    void UpdateUniversePool();
    void FillTranslateTable(int, int);
    void TranslateTopBoard(int, int);
    void TranslateBotBoard(int, int);
    void TranslateLeftBoard(int, int);
    void TranslateRightBoard(int, int);
    void TranslateCornerBoard(int, int);

    bool is_loop_;
    int it_cnt_;

    Universe support_;
    std::vector<Universe> universe_pool_;
    std::unordered_map<int, int> translate_table_;
};


#endif
