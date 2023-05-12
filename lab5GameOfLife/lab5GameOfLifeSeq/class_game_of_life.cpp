#include "class_game_of_life.h"
#include "stdlib.h"
#include <chrono>
#include <thread>

GameOfLife::GameOfLife(int u_len_y, int u_len_x) :
universe_pool_(constants::game_objects::START_NUM_UNIVERSE, Universe(u_len_y, u_len_x))
{}

void GameOfLife::StartGame()
{
    int it_cnt = 0;
    bool is_loop = false;

    universe_pool_.back().BornCells();

    //std::cout << std::endl;
    while(!is_loop && it_cnt++ < constants::game_logic::ITER_LIMIT)
    {
        //ShowLastUniverse();
        CopyCurrentUniverse();
        GetNextCellGeneration();
        is_loop = IsAnyMatchUniverse();
        UpdateUniversePool();
    }
    std::cout << "iterations to loop: " << it_cnt << std::endl;
}

void GameOfLife::CopyCurrentUniverse()
{
    support_ = universe_pool_.back();
}

void GameOfLife::GetNextCellGeneration()
{
    for (auto i = 0; i < support_.pool_row_; ++i)
    {
        for (auto j = 0; j < support_.pool_col_; ++j)
        {
            ChangeCellState(support_.cell_pool_[i*support_.pool_col_+j]);
        }
    }
    support_.UpdateCellNeighbours();
}

void GameOfLife::ChangeCellState(Cell& cell)
{
    if (cell.IsAlive())
    {
        if (cell.GetNumAliveNeighbour() < 2 || cell.GetNumAliveNeighbour() > 3)
        {
            cell.ChangeLifeStage(constants::game_objects::DEAD);
        }
    }
    else if (cell.GetNumAliveNeighbour() == 3)
    {
        cell.ChangeLifeStage(constants::game_objects::ALIVE);
    }
}

bool GameOfLife::IsAnyMatchUniverse() const
{
    return std::ranges::any_of(universe_pool_,
                               [this](const Universe& u)
                               { return GameOfLife::IsEqualUniverse(u); });
}

bool GameOfLife::IsEqualUniverse(const Universe& u) const
{
    for (auto i = 0; i < u.pool_size_; ++i)
    {
        if (u.cell_pool_[i] != support_.cell_pool_[i])
            return false;
    }
    return true;
}

void GameOfLife::UpdateUniversePool()
{
    if (universe_pool_.size() == universe_pool_.capacity())
    {
        universe_pool_.reserve(universe_pool_.size()+constants::game_objects::NEW_VECTOR_CAP);
    }
     universe_pool_.emplace_back(support_);
}

void GameOfLife::ShowLastUniverse() const
{
    universe_pool_.back().ShowUniverse();
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    std::cout.flush();
    std::system("clear");

    std::cout << std::endl;
}

void GameOfLife::EndGame()
{
    universe_pool_.clear();
    universe_pool_.emplace_back(support_.pool_row_, support_.pool_col_);
}