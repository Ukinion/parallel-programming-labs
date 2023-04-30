#ifndef LAB5GAMEOFLIFE_CLASS_UNIVERSE_H
#define LAB5GAMEOFLIFE_CLASS_UNIVERSE_H

#include <vector>
#include <unordered_map>
#include "class_cell.h"

class Universe
{
public:
    friend GameController;
    Universe(int, int);
    Universe(int, int, std::vector<Cell>&, std::unordered_map<int,int>&);

    void BornCells();
    void UpdateCellNeighbours();
    void ShowUniverse() const;
private:
    void DistributePlacesForCells();
    void FillTranslateTable();
    void TranslateTopBoard();
    void TranslateBotBoard();
    void TranslateLeftBoard();
    void TranslateRightBoard();
    void TranslateCornerBoard();
    bool IsBoundaryCell(const Cell&);
    int ScanCellEnvironment(const Cell&);

    std::vector<Cell> cell_pool_;
    std::unordered_map<int, int> translate_table_;
    int pool_row_;
    int pool_col_;
    int pool_size_;
};

#endif
