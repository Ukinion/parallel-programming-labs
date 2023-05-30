#pragma once

class Cell {
public:
    Cell() = default;
    Cell(int, int, bool);

    void Born();
    void Die();
    friend bool operator!=(const Cell&, const Cell&);

    int GetCoordX() const;
    int GetCoordY() const;
    bool IsAlive() const;
private:
    int x_;
    int y_;
    bool is_alive_;
};

bool operator==(const Cell&, const Cell&);