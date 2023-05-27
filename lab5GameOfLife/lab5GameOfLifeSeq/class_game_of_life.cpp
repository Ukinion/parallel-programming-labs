#include "class_game_of_life.h"
#include "stdlib.h"
#include <chrono>
#include <thread>

GameOfLife::GameOfLife(int game_field_row, int game_field_col) :
it_cnt_(0), is_loop_(false)
{
    FillTranslateTable(game_field_row, game_field_col);
}

void GameOfLife::FillTranslateTable(int row, int col)
{
    TranslateTopBoard(row, col);
    TranslateBotBoard(row, col);
    TranslateLeftBoard(row, col);
    TranslateRightBoard(row, col);
    TranslateCornerBoard(row, col);
}

void GameOfLife::TranslateTopBoard(int row, int col)
{
    for (auto x = 1; x < col+1; ++x)
    {
        translate_table_[x] = (row-1)*col+x-1;
    }
}

void GameOfLife::TranslateBotBoard(int row, int col)
{
    int y = (row+2)*(col+2)-(col+2);
    for (auto x = 1; x < col+1; ++x)
    {
        translate_table_[y+x] = x-1;
    }
}

void GameOfLife::TranslateLeftBoard(int row, int col)
{
    for (auto y = 1; y < row+1; ++y)
    {
        translate_table_[y*(col+2)] = (y-1)*col+col-1;
    }
}

void GameOfLife::TranslateRightBoard(int row, int col)
{
    int x = col+1;
    for (auto y = 1; y < row+1; ++y)
    {
        translate_table_[y*(col+2)+x] = (y-1)*col;
    }
}

void GameOfLife::TranslateCornerBoard(int row, int col)
{
    translate_table_[0] = row*col-1;
    translate_table_[col+1] = row*col-col;
    translate_table_[(row+2)*(col+2)-(col+2)] = col-1;
    translate_table_[(row+2)*(col+2)-1] = 0;
}

void GameOfLife::StartNewGame()
{

    universe_pool_.back().BornCells();

    //std::cout << std::endl;
    while(!is_loop_ && it_cnt_++ < constants::game_logic::ITER_LIMIT)
    {
        //ShowLastUniverse();
        CopyCurrentUniverse();
        GetNextCellGeneration();
        is_loop_ = IsAnyMatchUniverse();
        UpdateUniversePool();
    }
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
    if (is_loop_)
    {
        std::cout << "Loop was found!" << std::endl;
        std::cout << "iterations to loop: " << it_cnt_ << std::endl;
    }
    else
    {
        std::cout << "Reached iteration limit: " << constants::game_logic::ITER_LIMIT << std::endl;
    }
    universe_pool_.clear();
    universe_pool_.emplace_back(support_);
}

void GameOfLife::ShowTranslateTable() const
{
    for (std::pair<const int, int> i : translate_table_)
    {
        std::cout << "outer cell: " << i.first << " | ";
        std::cout << "inner cell: " << i.second << std::endl;
    }
}