#include "class_game_controller.h"

GameController::GameController(int u_len_y, int u_len_x) :
pool_size_(constants::game_objects::START_NUM_UNIVERSE),
universe_pool_(constants::game_objects::START_NUM_UNIVERSE, Universe(u_len_y, u_len_x))
{}

void GameController::StartGame()
{
    int it_cnt = 0;
    bool is_loop = false;

    universe_pool_.back().BornCells();

    while(!is_loop || it_cnt < constants::game_logic::ITER_LIMIT)
    {
        ShowLastUniverse();
        CopyCurrentUniverse();
        GetNextCellGeneration();
        is_loop = CompareCurUniverseWithPrev();
        UpdateUniversePool();
        ++it_cnt;
    }
}

void GameController::CopyCurrentUniverse()
{
    support_ = universe_pool_.back();
}

void GameController::GetNextCellGeneration()
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

void GameController::ChangeCellState(Cell& cell)
{
    if (cell.IsAlive())
    {
        if (cell.GetNumAliveNeighbour() < 2 || cell.GetNumAliveNeighbour() > 3)
        {
            cell.ChangeLifeStage(constants::game_objects::DEAD);
        }
    }
    else
    {
        if (cell.GetNumAliveNeighbour() == 3)
        {
            cell.ChangeLifeStage(constants::game_objects::ALIVE);
        }
    }
}

bool GameController::CompareCurUniverseWithPrev() const
{
    return std::ranges::any_of(universe_pool_,
                               [this](const Universe& u)
                               { return GameController::IsEqualUniverse(u); });
}

bool GameController::IsEqualUniverse(const Universe& u) const
{
    for (auto i = 0; i < u.pool_size_; ++i)
    {
        if (u.cell_pool_[i] != support_.cell_pool_[i])
            return false;
    }
    return true;
}

void GameController::UpdateUniversePool()
{
    if (universe_pool_.size() == universe_pool_.capacity())
    {
        universe_pool_.reserve(universe_pool_.size()+constants::game_objects::NEW_VECTOR_CAP);
    }
     universe_pool_.emplace_back(support_);
}

void GameController::ShowLastUniverse() const
{
    universe_pool_.back().ShowUniverse();
    std::cout << std::endl;
}