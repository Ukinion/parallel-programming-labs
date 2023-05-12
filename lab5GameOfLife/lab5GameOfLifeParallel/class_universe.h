#ifndef LAB5GAMEOFLIFEPARALLEL_CLASS_UNIVERSE_H
#define LAB5GAMEOFLIFEPARALLEL_CLASS_UNIVERSE_H


#include <vector>
#include <unordered_map>
#include "class_cell.h"

class Universe
{
public:
    friend GameOfLife;
    Universe();
    Universe(int, int);
    Universe(int, int, std::vector<Cell>&, std::unordered_map<int,int>&);
    Universe(const Universe&);

    Universe& operator=(const Universe&);

    void BornCells();
    void UpdateCellNeighbours();
    void ShowUniverse() const;
    void ShowArrayIndexBoard() const;
    void ShowTranslateTable() const;
    void ShowNeighbours() const;
private:
    void DistributePlacesForCells();
    void FillTranslateTable();
    void TranslateTopBoard();
    void TranslateBotBoard();
    void TranslateLeftBoard();
    void TranslateRightBoard();
    void TranslateCornerBoard();
    int ScanCellEnvironment(const Cell&);
    int GetRoundBoundaryCell(const Cell&);
    int GetRoundCommonCell(const Cell&);
    bool IsBoundaryCell(const Cell&) const;
    bool IsNeighbourTranslated(int, int, const Cell&);
    bool IsTranslatedNeighbourAlive(int, int, const Cell&);
    bool IsNeighbourAlive(int, int, const Cell&);

    std::vector<Cell> cell_pool_;
    std::unordered_map<int, int> translate_table_;

    int pool_row_;
    int pool_col_;
    int pool_size_;

    int fic_row_;
    int fic_col_;
    int fic_size_;
};



#endif
