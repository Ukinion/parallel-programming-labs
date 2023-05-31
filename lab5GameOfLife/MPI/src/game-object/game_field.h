#pragma once

#include <vector>
#include <iostream>

#include "../constants.h"
#include "../exception/invalid_option_exception.h"
#include "../exception/out_of_exception.h"
#include "../exception/not_comparable_exception.h"

class GameField {
public:
    GameField() = default;
    GameField(const GameField&) = default;
    GameField& operator=(const GameField&) = default;
    void InitGameField(int, int);
    void CreateGlider(int, int);
    bool IsOutOfField(int x, int y) const;
    void NextGeneration(const GameField&);
    int CountAliveNeighbours(int, int, const GameField&) const;
    friend bool operator==(const GameField&, const GameField&);
    void PrintField() const;
    std::vector<char> GetField() const;
    void SetNewField(const std::vector<char>&, int ,int);
private:
    bool IsInvalidFieldOptions(int, int) const;
    static bool IsNotComparable(const GameField&, const GameField&);
    int mod(int, int) const;

    static std::vector<char> prev_row_;
    static std::vector<char> next_row_;
    std::vector<char> game_field_;
    int row_;
    int col_;
};