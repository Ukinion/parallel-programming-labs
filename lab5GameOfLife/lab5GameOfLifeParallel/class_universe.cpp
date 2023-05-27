#include "class_universe.h"

Universe::Universe() :
pool_row_(0), pool_col_(0), pool_size_(0),
fic_row_(0), fic_col_(0), fic_size_(0)
{}

Universe::Universe(int row, int col) :
pool_row_(row), pool_col_(col), pool_size_(row*col), cell_pool_(row*col),
fic_row_(row+2), fic_col_(col+2), fic_size_((row+2)*(col+2))
{
    DistributePlacesForCells();
    FillTranslateTable();
}

Universe::Universe(const Universe& u) = default;

void Universe::DistributePlacesForCells() {
    for (auto i = 0; i < pool_row_; ++i) {
        for (auto j = 0; j < pool_col_; ++j) {
            cell_pool_[i*pool_col_+j].MoveCell(j, i);
        }
    }
}

void Universe::FillTranslateTable() {
    TranslateTopBoard();
    TranslateBotBoard();
    TranslateLeftBoard();
    TranslateRightBoard();
    TranslateCornerBoard();
}

void Universe::TranslateTopBoard() {
    for (auto x = 1; x < fic_col_-1; ++x) {
        translate_table_[x] = (pool_row_-1)*pool_col_+x-1;
    }
}

void Universe::TranslateBotBoard() {
    int y = fic_size_-fic_col_;
    for (auto x = 1; x < fic_col_-1; ++x) {
        translate_table_[y+x] = x-1;
    }
}

void Universe::TranslateLeftBoard() {
    for (auto y = 1; y < fic_row_-1; ++y) {
        translate_table_[y*fic_col_] = (y-1)*pool_col_+pool_col_-1;
    }
}

void Universe::TranslateRightBoard() {
    int x = fic_col_-1;
    for (auto y = 1; y < fic_row_-1; ++y) {
        translate_table_[y*fic_col_+x] = (y-1)*pool_col_;
    }
}

void Universe::TranslateCornerBoard() {
    translate_table_[0] = pool_size_-1;
    translate_table_[fic_col_-1] = pool_size_-pool_col_;
    translate_table_[fic_size_-fic_col_] = pool_col_-1;
    translate_table_[fic_size_-1] = 0;
}

void Universe::BornCells() {
//  TEST_CASE | Glider
    cell_pool_[1].ChangeLifeStage(constants::game_objects::ALIVE);
    cell_pool_[pool_col_+2].ChangeLifeStage(constants::game_objects::ALIVE);
    cell_pool_[2*pool_col_].ChangeLifeStage(constants::game_objects::ALIVE);
    cell_pool_[2*pool_col_+1].ChangeLifeStage(constants::game_objects::ALIVE);
    cell_pool_[2*pool_col_+2].ChangeLifeStage(constants::game_objects::ALIVE);

    UpdateCellNeighbours();
}

void Universe::UpdateCellNeighbours() {
    for (auto i = 0; i < pool_size_; ++i) {
        cell_pool_[i].ChangeNumAliveNeighbour(ScanCellEnvironment(cell_pool_[i]));
    }
}

int Universe::ScanCellEnvironment(const Cell& cell) {
    if (IsBoundaryCell(cell)) {
        return GetRoundBoundaryCell(cell);
    } else {
        return GetRoundCommonCell(cell);
    }
}

int Universe::GetRoundBoundaryCell(const Cell& cell) {
    int num_neighbours = 0;
    for (auto i = -1; i < constants::game_logic::LINE_ENVIRONMENT; ++i) {
        for (auto j = -1; j < constants::game_logic::LINE_ENVIRONMENT; ++j) {
            if (IsNeighbourTranslated(i, j, cell)) {
                if (IsTranslatedNeighbourAlive(i, j, cell)) {
                    ++num_neighbours;
                }
            } else if (IsNeighbourAlive(i, j, cell)) {
                ++num_neighbours;
            }
        }
    }
    if (cell.IsAlive()) {
        return num_neighbours-1;
    } else {
        return num_neighbours;
    }
}

bool Universe::IsNeighbourTranslated(int i, int j, const Cell& cell) {
    return translate_table_.contains((cell.y_+i+1)*fic_col_+cell.x_+1+j);
}

bool Universe::IsTranslatedNeighbourAlive(int i, int j, const Cell& cell) {
    return cell_pool_[translate_table_[(cell.y_+i+1)*fic_col_+cell.x_+1+j]].IsAlive();
}

bool Universe::IsNeighbourAlive(int i, int j, const Cell& cell) {
    return cell_pool_[(cell.y_+i)*pool_col_+cell.x_+j].IsAlive();
}

int Universe::GetRoundCommonCell(const Cell& cell) {
    int num_neighbours = 0;
    for (auto i = -1; i < constants::game_logic::LINE_ENVIRONMENT; ++i) {
        for (auto j = -1; j < constants::game_logic::LINE_ENVIRONMENT; ++j) {
            if (IsNeighbourAlive(i, j, cell)) {
                ++num_neighbours;
            }
        }
    }
    if (cell.IsAlive()) {
        return num_neighbours-1;
    } else {
        return num_neighbours;
    }
}

bool Universe::IsBoundaryCell(const Cell& cell) const {
    if (cell.x_ == pool_col_-1 || cell.x_ == 0 || cell.y_ == pool_row_-1 || cell.y_ == 0) {
        return true;
    } else {
        return false;
    }
}

void Universe::ShowUniverse() const {
    for (auto i = 0; i < pool_row_; ++i) {
        for (auto j = 0; j < pool_col_; ++j) {
            std::cout << cell_pool_[i*pool_col_+j].GetLifeStage() << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Universe::ShowNeighbours() const {
    for (auto i = 0; i < pool_row_; ++i) {
        for (auto j = 0; j < pool_col_; ++j) {
            std::cout << cell_pool_[i*pool_col_+j].GetNumAliveNeighbour() << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Universe::ShowArrayIndexBoard() const {
    for (auto i = 0; i < fic_row_; ++i) {
        for (auto j = 0; j < fic_col_; ++j) {
            std::cout << i*fic_col_+j << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (auto i = 0; i < pool_col_; ++i) {
        for (auto j = 0; j < pool_row_; ++j) {
            std::cout << i*pool_col_+j << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Universe::ShowTranslateTable() const {
    for (std::pair<const int, int> i : translate_table_) {
        std::cout << "outer cell: " << i.first << " | ";
        std::cout << "inner cell: " << i.second << std::endl;
    }
}

Universe& Universe::operator=(const Universe& u) = default;
