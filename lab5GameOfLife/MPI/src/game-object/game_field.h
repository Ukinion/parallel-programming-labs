#pragma once

#include <vector>
#include <iostream>

#include "../constants.h"
#include "../exception/invalid_option_exception.h"
#include "../exception/out_of_exception.h"
#include "../exception/not_comparable_exception.h"
#include "cell.h"

class GameField {
public:
    GameField() = default;
    GameField(const GameField&) = default;
    void InitGameField(int, int);
    void CreateGlider(int, int);
    bool IsOutOfField(int x, int y) const;
    bool IsOutOfField(const Cell&) const;
    void NextGeneration(const GameField&);
    int CountAliveNeighbours(const Cell&, const GameField&) const;
    friend bool operator==(const GameField&, const GameField&);
    void PrintField() const;
private:
    bool IsInvalidFieldOptions(int, int) const;
    static bool IsNotComparable(const GameField&, const GameField&);
    int mod(int, int) const;

    std::vector<Cell> game_field_;
    int row_;
    int col_;
};