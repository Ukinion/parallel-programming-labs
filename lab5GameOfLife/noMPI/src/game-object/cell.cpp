#include "cell.h"

Cell::Cell(int x, int y, bool life_stage) {
    x_ = x;
    y_ = y;
    is_alive_ = life_stage;
}

void Cell::Born() {
    is_alive_ = true;
}

void Cell::Die() {
    is_alive_ = false;
}

bool operator!=(const Cell& this_cell, const Cell& another_cell) {
    return this_cell.is_alive_ != another_cell.is_alive_  ||
           this_cell.x_ != another_cell.x_ ||
           this_cell.y_ != another_cell.y_;
}

bool operator==(const Cell& this_cell, const Cell& another_cell) {
    return !(this_cell != another_cell);
}

bool Cell::IsAlive() const {
    return is_alive_;
}

int Cell::GetCoordX() const {
    return x_;
}

int Cell::GetCoordY() const {
    return y_;
}

