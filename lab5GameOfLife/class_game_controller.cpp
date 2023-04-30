#include "class_game_controller.h"

GameController::GameController(int u_len_y, int u_len_x) :
pool_size_(constants::game_objects::START_NUM_UNIVERSE),
universe_pool_(constants::game_objects::START_NUM_UNIVERSE, Universe(u_len_y, u_len_x)),
support_(u_len_x, u_len_y)
{}

void GameController::StartGame()
{
    int count = 0;
    bool is_loop = false;

    while(!is_loop)
    {
        CopyCurrentUniverse();
        ChangeCurrentUniverse();

    }
}

void GameController::CopyCurrentUniverse()
{
    support_.cell_pool_ = universe_pool_.back().cell_pool_;
    support_.translate_table_ = universe_pool_.back().translate_table_;
}

void GameController::ChangeCurrentUniverse()
{
    GetNextCellGeneration(universe_pool_.back());
    CopyCurrentUniverse();
}

void GameController::GetNextCellGeneration(const Universe& cur_u)
{

}

bool GameController::CompareCurUniverseWithPrev() const
{

}

bool GameController::IsEqualUniverse() const
{

}