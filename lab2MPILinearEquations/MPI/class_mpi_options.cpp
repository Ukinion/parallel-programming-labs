#include "class_mpi_matrix.h"

void MPI_Matrix::MPI_Options::SetupOptions(int row, int col)
{
    int rows_per_proc = row / size_;
    int remain_per_proc = row - rows_per_proc * size_;
    int offset = 0;

    cnts_per_proc_ = new int[size_];
    displs_ = new int[size_]{};
    rows_per_proc_ = new int[size_];

    for (auto i = 0; i < size_; ++i)
    {
        if (remain_per_proc)
        {
            cnts_per_proc_[i] = (rows_per_proc + 1) * col;
            rows_per_proc_[i] = rows_per_proc + 1;
            --remain_per_proc;
        }
        else
        {
            cnts_per_proc_[i] = rows_per_proc * col;
            rows_per_proc_[i] = rows_per_proc;
        }
        displs_[i] += offset;
        offset += cnts_per_proc_[i];
    }

    local_start_ = displs_[rank_];
    local_end_ = local_start_ + cnts_per_proc_[rank_];
}


void MPI_Matrix::MPI_Options::CopyMemory(const MPI_Options& options)
{
    std::memcpy(displs_, options.displs_, sizeof(int) * size_);
    std::memcpy(cnts_per_proc_, options.cnts_per_proc_, sizeof(int) * size_);
    std::memcpy(rows_per_proc_, options.cnts_per_proc_, sizeof(int) * size_);
}

MPI_Matrix::MPI_Options::MPI_Options(int row, int col)
{
    MPI_Comm_size(MPI_COMM_WORLD, &size_);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_);

    SetupOptions(row, col);
}

MPI_Matrix::MPI_Options::MPI_Options(const MPI_Options& options) :
size_ (options.size_), rank_ (options.rank_),
local_start_ (options.local_start_), local_end_ (options.local_end_)
{
    displs_ = new int[size_];
    cnts_per_proc_ = new int[size_];
    rows_per_proc_ = new int[size_];
    CopyMemory(options);
}

MPI_Matrix::MPI_Options::~MPI_Options()
{
    delete[] displs_;
    delete[] cnts_per_proc_;
    delete[] rows_per_proc_;
}

MPI_Matrix::MPI_Options& MPI_Matrix::MPI_Options::operator=(const MPI_Options& options)
{
    if (this != &options)
    {
    size_ = options.size_;
    rank_ = options.rank_;
    local_start_ = options.local_start_;
    local_end_ = options.local_end_;
    CopyMemory(options);
    }
    return *this;
}


