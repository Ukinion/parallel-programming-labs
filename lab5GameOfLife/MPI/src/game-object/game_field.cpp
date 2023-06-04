#include "mpi.h"

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
    prev_generation.SendRowPrevProcess();
    prev_generation.SendRowNextProcess();
    ReceiveNextProcessRow();
    ReceivePrevProcessRow();
    UpdateFieldMiddle(prev_generation.game_field_);
    MPI_Wait(mpi_scheduler_.recv_first_row, MPI_STATUS_IGNORE);
    UpdateFieldTop(prev_generation.game_field_);
    MPI_Wait(mpi_scheduler_.recv_last_row, MPI_STATUS_IGNORE);
    UpdateFieldBottom(prev_generation.game_field_);
    MPI_Wait(mpi_scheduler_.send_first_row, MPI_STATUS_IGNORE);
    MPI_Wait(mpi_scheduler_.send_last_row, MPI_STATUS_IGNORE);
}

void GameField::SendRowPrevProcess() const {
    MPI_Isend(&game_field_[0], col_, MPI_CHAR,
              mod(mpi_scheduler_.rank - 1, GameField::DataMPI::size),
              constants::mpi::SEND_PREV, MPI_COMM_WORLD, mpi_scheduler_.send_last_row);
}

void GameField::SendRowNextProcess() const {
    MPI_Isend(&game_field_[game_field_.size() - 1], col_, MPI_CHAR,
              mod(mpi_scheduler_.rank + 1, GameField::DataMPI::size),
              constants::mpi::SEND_NEXT, MPI_COMM_WORLD, mpi_scheduler_.send_first_row);
}

void GameField::ReceiveNextProcessRow() {
    MPI_Irecv(&GameField::DataMPI::last_row[0], col_, MPI_CHAR,
              mod(mpi_scheduler_.rank + 1 , GameField::DataMPI::size),
              constants::mpi::SEND_PREV, MPI_COMM_WORLD, mpi_scheduler_.recv_last_row);
}

void GameField::ReceivePrevProcessRow() {
    MPI_Irecv(&GameField::DataMPI::first_row[0], col_, MPI_CHAR,
              mod(mpi_scheduler_.rank - 1 , GameField::DataMPI::size),
              constants::mpi::SEND_NEXT, MPI_COMM_WORLD, mpi_scheduler_.recv_first_row);
}

void GameField::UpdateFieldMiddle(const std::vector<char>& prev_field) {
    int num_alive_neigh;
    for (auto y = 1; y < row_ - 1; ++y) {
        for (auto x = 0; x < col_; ++x)
            if (prev_field[y * col_ + x] == constants::game::LIVING_CELL) {
                num_alive_neigh = CountAliveNeighbours(x, y, prev_field);
                if (num_alive_neigh < 2 || num_alive_neigh > 3) {
                    this->game_field_[y * col_ + x] = constants::game::DEAD_CELL;
                }
            } else if (CountAliveNeighbours(x, y,prev_field) == 3) {
                this->game_field_[y * col_ + x] = constants::game::LIVING_CELL;
            }
    }
}

int GameField::CountAliveNeighbours(int x, int y, const std::vector<char>& this_field) const {
    int num_alive_neigh = 0;
    for (auto i = -1; i < 2; ++i) {
        for (auto j = -1; j < 2; ++j) {
            if (this_field[mod(y + i, row_) * col_+ mod(x + j, col_)]
                == constants::game::LIVING_CELL) {
                num_alive_neigh++;
            }
        }
    }
    if (this_field[y * col_ + x] == constants::game::LIVING_CELL) {
        return num_alive_neigh - 1;
    }
    return num_alive_neigh;
}

void GameField::UpdateFieldTop(const std::vector<char>& prev_field) {
    int num_alive_neigh;
    for (auto x = 0; x < col_; ++x) {
        if (prev_field[x] == constants::game::LIVING_CELL) {
            num_alive_neigh = CountAliveNeighbours(x, 0,
                                                   prev_field, 0, GameField::DataMPI::first_row);
            if (num_alive_neigh < 2 || num_alive_neigh > 3) {
                this->game_field_[x] = constants::game::DEAD_CELL;
            }
        } else if (CountAliveNeighbours(x, 0,
                                        prev_field, 0, GameField::DataMPI::first_row) == 3) {
            this->game_field_[x] = constants::game::LIVING_CELL;
        }
    }
}

void GameField::UpdateFieldBottom(const std::vector<char>& prev_field) {
    int num_alive_neigh;
    for (auto x = 0; x < col_; ++x) {
        if (prev_field[(prev_field.size() - 1) * col_ + x] == constants::game::LIVING_CELL) {
            num_alive_neigh = CountAliveNeighbours(x, prev_field.size() - 1,
                                                   prev_field, -1, GameField::DataMPI::last_row);
            if (num_alive_neigh < 2 || num_alive_neigh > 3) {
                this->game_field_[(game_field_.size() - 1) * col_ + x] = constants::game::DEAD_CELL;
            }
        } else if (CountAliveNeighbours(x, prev_field.size() - 1,
                                        prev_field, -1, GameField::DataMPI::last_row) == 3) {
            this->game_field_[(game_field_.size() - 1) * col_ + x] = constants::game::LIVING_CELL;
        }
    }
}

int GameField::CountAliveNeighbours(int x, int y, const std::vector<char>& this_field,
        int start, const std::vector<char>& sent_row) const {
    int num_alive_neigh = 0;
    for (auto i = start; i < 2; ++i) {
        for (auto j = -1; j < 2; ++j) {
            if (this_field[mod(y + i, row_) * col_+ mod(x + j, col_)]
                == constants::game::LIVING_CELL) {
                num_alive_neigh++;
            }
        }
    }
    for (auto j = -1; j < 2; ++j) {
        if (sent_row[mod(x + j, col_)] == constants::game::LIVING_CELL) {
            num_alive_neigh++;
        }
    }
    if (this_field[y * col_ + x] == constants::game::LIVING_CELL) {
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

void GameField::SetNewField(const std::vector<char>& new_field, int row, int col, int rank, int size) {
    if (IsInvalidFieldOptions(row, col)) {
        throw invalid_option_exception("\n\tError: Invalid field options.");
    }
    game_field_ = new_field;
    row_ = row;
    col_ = col;
    InitMetaMPI(rank, size);
}

void GameField::InitMetaMPI(int rank, int size) {
    mpi_scheduler_.rank = rank;
    GameField::DataMPI::size = size;
    GameField::DataMPI::first_row = std::vector<char>(col_);
    GameField::DataMPI::last_row = std::vector<char>(col_);
    mpi_scheduler_.send_first_row = nullptr;
    mpi_scheduler_.send_last_row = nullptr;
    mpi_scheduler_.recv_first_row = nullptr;
    mpi_scheduler_.recv_last_row = nullptr;
}
