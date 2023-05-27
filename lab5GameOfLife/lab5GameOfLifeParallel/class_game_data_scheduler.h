#ifndef LAB5GAMEOFLIFEPARALLEL_CLASS_GAME_DATA_SCHEDULER_H
#define LAB5GAMEOFLIFEPARALLEL_CLASS_GAME_DATA_SCHEDULER_H

#include "class_universe.h"

class GameDataScheduler {
public:
    friend GameOfLife;
    GameDataScheduler(Universe* u) {
        MPI_Comm_size(MPI_COMM_WORLD, &num_proc_);
        DetermineDataForScatter(u->cell_pool_, u->pool_row_, u->pool_col_);
        all_universe_ = u;
    }

    ~GameDataScheduler() {
        delete[] sendcount_;
        delete[] displs_;
        delete[] rows_per_proc_;
        delete all_universe_;
    }

private:
    void DetermineDataForScatter(const std::vector<Cell>& vec, int num_row, int num_col) {
        int rows_per_proc = num_proc_ / num_row;
        int remain_row = num_row - rows_per_proc * num_row;
        int offset = 0;
        sendcount_ = new int[num_proc_];
        displs_ = new int[num_proc_];
        rows_per_proc_ = new int[num_proc_];
        for (auto i = 0, sup_row = 1; i < num_proc_; ++i) {
            if (remain_row-- == 0) sup_row = 0;
            sendcount_[i] = num_col*(rows_per_proc+sup_row);
            rows_per_proc_[i] = rows_per_proc+sup_row+2;
            displs_[i] = offset;
            offset += sendcount_[i];
        }
    }

    Universe* all_universe_;
    int num_proc_;
    int* sendcount_;
    int* displs_;
    int* rows_per_proc_;
};


#endif
