#ifndef LAB5GAMEOFLIFE_CLASS_CELL_H
#define LAB5GAMEOFLIFE_CLASS_CELL_H

#include <iostream>
#include <algorithm>
#include "constants.h"


class GameController;
class GameOfLife;
class Universe;

class Cell
{
public:
    friend Universe;
    Cell() :
    x_(0), y_(0), num_alive_neighbours_(0), life_stage_ (false) {}

    Cell(int x, int y, bool is_alive) :
    x_(x), y_(y), num_alive_neighbours_(0), life_stage_(is_alive) {}

    void MoveCell(int x, int y)
    {
        x_ = x;
        y_ = y;
    }

    bool operator==(const Cell& cell) const
    {
        if (life_stage_ == cell.life_stage_ && x_ == cell.x_ &&
        y_ == cell.y_ && num_alive_neighbours_  == cell.num_alive_neighbours_)
            return true;
        else
            return false;
    }

    bool operator!=(const Cell& cell) const
    {
        return !(*this == cell);
    }

    void SetCoordinateX(int x) { x_ = x; }
    void SetCoordinateY(int y) { y_ = y; }
    void ChangeNumAliveNeighbour(int num_alive_neighbours) { num_alive_neighbours_ = num_alive_neighbours; }
    void ChangeLifeStage(bool new_life_stage) { life_stage_ = new_life_stage; }

    auto GetCoordinateX() const { return x_; }
    auto GetCoordinateY() const { return y_; }
    auto GetNumAliveNeighbour() const { return num_alive_neighbours_; }
    auto GetLifeStage() const { return life_stage_; }
    bool IsAlive() const { return life_stage_ == constants::game_objects::CellType::ALIVE; }

private:
    int x_;
    int y_;
    int num_alive_neighbours_;

    bool life_stage_;
};

#endif
