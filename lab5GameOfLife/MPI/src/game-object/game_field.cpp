#include "game_field.h"


void GameField::InitGameField(int row, int col) {
    if (IsInvalidFieldOptions(row, col)) {
        throw invalid_option_exception("\n\tError: Invalid field options.");
    }
    row_ = row;
    col_ = col;
    game_field_ = std::vector<char>(row * col, constants::game::DEAD_CELL);
}

bool GameField::IsInvalidFieldOptions(int row, int col) const {
    return row < constants::game::MIN_FIELD_SIZE ||
           col < constants::game::MIN_FIELD_SIZE;
}

void GameField::CreateGlider(int start_x, int start_y) {
    if (col_ < constants::game::GLIDER_REQ_SIZE ||
        row_ < constants::game::GLIDER_REQ_SIZE) {
        throw invalid_option_exception("\n\tError: glider cannot be create.");
    } else if (IsOutOfField(start_x, start_y)) {
        throw out_of_exception("\n\tInvalid start position: out of field.");
    }
    game_field_[mod(start_y, row_) * col_ + mod(start_x, col_)] = constants::game::LIVING_CELL;
    game_field_[mod(start_y + 1, row_) * col_ + mod(start_x + 1, col_)] = constants::game::LIVING_CELL;
    game_field_[mod(start_y + 2, row_) * col_ + mod(start_x, col_)] = constants::game::LIVING_CELL;
    game_field_[mod(start_y + 2, row_) * col_ + mod(start_x + 1, col_)] = constants::game::LIVING_CELL;
    game_field_[mod(start_y + 2, row_) * col_ + mod(start_x - 1, col_)] = constants::game::LIVING_CELL;
}

int GameField::mod(int a, int b) const {
    return (b + (a % b)) % b;
}

bool GameField::IsOutOfField(int x, int y) const {
    return x < -1 || x > col_ || y < -1 || y > row_;
}

void GameField::NextGeneration(const GameField& prev_generation) {
    int num_alive_neigh;
    for (auto y = 0; y < row_; ++y) {
        for (auto x = 0; x < col_; ++x)
        if (prev_generation.game_field_[y*col_+x] == constants::game::LIVING_CELL) {
            num_alive_neigh = CountAliveNeighbours(x, y, prev_generation);
            if (num_alive_neigh < 2 || num_alive_neigh > 3) {
                this->game_field_[y*col_+x] = constants::game::DEAD_CELL;
            }
        } else if (CountAliveNeighbours(x, y,prev_generation) == 3) {
            this->game_field_[y*col_+x] = constants::game::LIVING_CELL;
        }
    }
}

int GameField::CountAliveNeighbours(int x, int y, const GameField& this_field) const {
    int num_alive_neigh = 0;
    for (auto i = -1; i < 2; ++i) {
        for (auto j = -1; j < 2; ++j) {
            if (this_field.game_field_[mod(y + i, row_) * col_+ mod(x + j, col_)]
                == constants::game::LIVING_CELL) {
                num_alive_neigh++;
            }
        }
    }
    if (this_field.game_field_[y*col_+x] == constants::game::LIVING_CELL) {
        return num_alive_neigh - 1;
    }
    return num_alive_neigh;
}

bool operator==(const GameField& this_field, const GameField& another_field) {
    if (GameField::IsNotComparable(this_field, another_field)) {
        throw not_comparable_exception("\tFields are not comparable");
    }
    if (this_field.game_field_ != another_field.game_field_) {
        return false;
    }
    return true;
}

bool GameField::IsNotComparable(const GameField& this_field, const GameField& another_field) {
    return this_field.row_ != another_field.row_ || this_field.col_ != another_field.col_;
}

void GameField::PrintField() const {
    for (auto i = 0; i < row_; ++i) {
        for (auto j = 0; j < col_; ++j) {
            std::cout << game_field_[i*col_+j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector<char> GameField::GetField() const {
    return game_field_;
}

void GameField::SetNewField(const std::vector<char>& new_field, int row, int col) {
    if (IsInvalidFieldOptions(row, col)) {
        throw invalid_option_exception("\n\tError: Invalid field options.");
    }
    game_field_ = new_field;
    row_ = row;
    col_ = col;
    prev_row_ = std::vector<char>(col);
    next_row_ = std::vector<char>(col);
}
