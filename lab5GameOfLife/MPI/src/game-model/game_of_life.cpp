#include <algorithm>

#include "game_of_life.h"

void GameOfLife::StartGame() {
    iter_cnt_ = 0;
    work_time_ = MPI_Wtime();
    history_.reserve(constants::game::BASE_HISTORY_CAPACITY);
    GameProcess();
    work_time_ = MPI_Wtime() - work_time_;
}

void GameOfLife::GameProcess() {
    while ((iter_cnt_++) < constants::game::ITERATION_LIMIT) {
        UpdateHistory();
        if (SpawnNewGeneration()) break;
    }
}

void GameOfLife::UpdateHistory() {
    if (history_.size() == history_.capacity()) {
        history_.reserve(history_.size() + constants::game::NEW_CAPACITY_ADDITION);
    }
    history_.emplace_back(buffer_field_);
}

bool GameOfLife::SpawnNewGeneration() {
    buffer_field_.NextGeneration(history_.back());
    return IterateThroughHistory();
}

bool GameOfLife::IterateThroughHistory() const {
    return std::any_of(history_.begin(), history_.end(),
                       [this](const auto& past)
                       { return past == buffer_field_; });
}

void GameOfLife::EndGame() {
    if (iter_cnt_ < constants::game::ITERATION_LIMIT) {
        std::cout << "\n\tCongratulations!" << std::endl;
        std::cout << "\tLoop has benn detected on iteration: " << iter_cnt_ << std::endl;
    } else {
        std::cout << "\n\tGame Failed." << std::endl;
        std::cout << "\tReached iteration limit!" << std::endl;
    }
    std::cout << "\texecution time: " << work_time_ << std::endl;
}

void GameOfLife::SetField(const GameField& field) {
    buffer_field_ = field;
}
