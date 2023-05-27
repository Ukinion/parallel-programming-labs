#include "class_game_of_life.h"
#include <chrono>
#include <thread>

GameOfLife::GameOfLife(int u_len_y, int u_len_x) :
scheduler_(new Universe(u_len_y, u_len_x)),
it_cnt_(0), is_loop_(false)
{}

void GameOfLife::PrepareGameResource() {

}

void GameOfLife::StartNewGame() {
    universe_pool_.back().BornCells();

    //std::cout << std::endl;
    while(!is_loop_ && it_cnt_++ < constants::game_logic::ITER_LIMIT) {
        //ShowLastUniverse();
        CopyCurrentUniverse();
        GetNextCellGeneration();
        is_loop_ = IsAnyMatchUniverse();
        UpdateUniversePool();
    }
}

void GameOfLife::CopyCurrentUniverse() {
    universe_part_ = universe_pool_.back();
}

void GameOfLife::GetNextCellGeneration() {
    for (auto i = 0; i < universe_part_.pool_row_; ++i) {
        for (auto j = 0; j < universe_part_.pool_col_; ++j) {
            ChangeCellState(universe_part_.cell_pool_[i * universe_part_.pool_col_ + j]);
        }
    }
    universe_part_.UpdateCellNeighbours();
}

void GameOfLife::ChangeCellState(Cell& cell) {
    if (cell.IsAlive()) {
        if (cell.GetNumAliveNeighbour() < 2 || cell.GetNumAliveNeighbour() > 3) {
            cell.ChangeLifeStage(constants::game_objects::DEAD);
        }
    } else if (cell.GetNumAliveNeighbour() == 3) {
        cell.ChangeLifeStage(constants::game_objects::ALIVE);
    }
}

bool GameOfLife::IsAnyMatchUniverse() const {
    return std::ranges::any_of(universe_pool_,
                               [this](const Universe& u)
                               { return GameOfLife::IsEqualUniverse(u); });
}

bool GameOfLife::IsEqualUniverse(const Universe& u) const {
    for (auto i = 0; i < u.pool_size_; ++i) {
        if (u.cell_pool_[i] != universe_part_.cell_pool_[i]) {
            return false;
        }
    }
    return true;
}

void GameOfLife::UpdateUniversePool() {
    if (universe_pool_.size() == universe_pool_.capacity()) {
        universe_pool_.reserve(universe_pool_.size()+constants::game_objects::NEW_VECTOR_CAP);
    }
    universe_pool_.emplace_back(universe_part_);
}

void GameOfLife::ShowLastUniverse() const {
    universe_pool_.back().ShowUniverse();
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    std::cout.flush();
    std::system("clear");

    std::cout << std::endl;
}

void GameOfLife::EndGame() {
    if (is_loop_) {
        std::cout << "Loop was found!" << std::endl;
        std::cout << "iterations to loop: " << it_cnt_ << std::endl;
    } else {
        std::cout << "Reached iteration limit: " << constants::game_logic::ITER_LIMIT << std::endl;
    }
    universe_pool_.clear();
    universe_pool_.emplace_back(universe_part_);
}