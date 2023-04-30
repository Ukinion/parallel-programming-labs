#include "class_universe.h"

Universe::Universe(int row, int col) :
pool_row_(row), pool_col_(col), pool_size_(row*col), cell_pool_(row*col)
{
    DistributePlacesForCells();
    FillTranslateTable();
}

Universe::Universe(int row, int col, std::vector<Cell>& pool, std::unordered_map<int,int>& table) :
pool_row_(row), pool_col_(col), pool_size_(row*col), cell_pool_(pool), translate_table_(table)
{}

void Universe::DistributePlacesForCells()
{
    for (auto i = 0; i < pool_row_; ++i)
    {
        for (auto j = 0; j < pool_col_; ++j)
        {
            cell_pool_[i*pool_col_+j].MoveCell(i, j);
        }
    }
}

void Universe::FillTranslateTable()
{
    TranslateTopBoard();
    TranslateBotBoard();
    TranslateLeftBoard();
    TranslateRightBoard();
    TranslateCornerBoard();
}

void Universe::TranslateTopBoard()
{
    int y = -pool_col_;
    for (auto x = 0; x < pool_col_-2; ++x)
    {
        translate_table_[y+x] = (pool_row_-1)*pool_col_+x;
    }
}

void Universe::TranslateBotBoard()
{
    int y = pool_size_;
    for (auto x = 0; x < pool_col_-2; ++x)
    {
        translate_table_[y+x] = x;
    }
}

void Universe::TranslateLeftBoard()
{
    int x = -1;
    for (auto y = 0; y < pool_row_-2; ++y)
    {
        translate_table_[y*pool_col_+x] = y*pool_col_+pool_col_-1;
    }
}

void Universe::TranslateRightBoard()
{
    int x = pool_col_;
    for (auto y = 0; y < pool_row_-2; ++y)
    {
        translate_table_[y*pool_col_+x] = y*pool_col_;
    }
}

void Universe::TranslateCornerBoard()
{
    translate_table_[-pool_col_-1] = pool_size_-1;
    translate_table_[pool_size_-1] = pool_col_-1;
    translate_table_[0] = pool_size_-pool_col_;
    translate_table_[pool_size_+pool_col_] = 0;
}

void Universe::BornCells()
{
//  TEST_CASE | Glider
    cell_pool_[1].ChangeLifeStage(true);
    cell_pool_[pool_col_+2].ChangeLifeStage(true);
    cell_pool_[2*pool_col_].ChangeLifeStage(true);
    cell_pool_[2*pool_col_+1].ChangeLifeStage(true);
    cell_pool_[2*pool_col_+2].ChangeLifeStage(true);

    UpdateCellNeighbours();
}

void Universe::UpdateCellNeighbours()
{
    for (auto i = 0; i < pool_size_; ++i)
    {
        cell_pool_[i].ChangeNumAliveNeighbour(ScanCellEnvironment(cell_pool_[i]));
    }
}

int Universe::ScanCellEnvironment(const Cell& cell)
{
    int num_neighbours = 0;
    if (IsBoundaryCell(cell))
    {
        for (auto i = -1; i < constants::game_logic::LINE_ENVIRONMENT; ++i)
        {
            for (auto j = -1; j < constants::game_logic::LINE_ENVIRONMENT; ++j)
            {
                if (translate_table_.contains((cell.y_+i)*pool_col_+j))
                {
                    if (cell_pool_[translate_table_[(cell.y_+i)*pool_col_+j]].life_stage_)
                        num_neighbours++;
                }
                else
                {
                    if (cell_pool_[(cell.y_+i)*pool_col_+j].life_stage_)
                        ++num_neighbours;
                }
            }
        }
    }
    else
    {
        for (auto i = -1; i < constants::game_logic::LINE_ENVIRONMENT; ++i)
        {
            for (auto j = -1; j < constants::game_logic::LINE_ENVIRONMENT; ++j)
            {
                if (cell_pool_[(cell.y_+i)*pool_col_+j].life_stage_)
                    ++num_neighbours;
            }
        }
    }
    return num_neighbours-1;
}

bool Universe::IsBoundaryCell(const Cell& cell)
{
    if (cell.x_ == pool_col_-1 || cell.x_ == 0 ||
        cell.y_ == pool_row_-1 || cell.y_ == 0) return true;
    else return false;
}


void Universe::ShowUniverse() const
{
    for (auto i = 0; i < pool_row_; ++i)
    {
        for (auto j = 0; j < pool_col_; ++j)
        {
            std::cout << cell_pool_[i*pool_col_+j].GetLifeStage() << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
