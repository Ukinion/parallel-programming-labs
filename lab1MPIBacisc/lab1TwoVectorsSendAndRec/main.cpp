#include <iostream>
#include <vector>
#include <random>
#include <mpi.h>

namespace constants
{
    constexpr auto kRandomRange = 20;
    constexpr auto kTestVecSize = 90000;
    constexpr auto kTag = 123;
    constexpr auto kSource = 0;
    constexpr auto kTimes = 5;
}

double* InitVector(size_t size)
{
    auto nw_vec = new double[size];

    std::random_device rnd_dev;
    std::mt19937 eng(rnd_dev());
    std::uniform_real_distribution<double> dist(-constants::kRandomRange, constants::kRandomRange);

    for(size_t i = 0; i < size; ++i) { nw_vec[i] = dist(eng); }
    return nw_vec;
}

double Algorithm(const double* vec1, const double* vec2, int count)
{
    double s = 0;

    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < constants::kTestVecSize; ++j) { s += vec1[i] * vec2[j]; }
    }
    return s;
}

double StartComputing(int size, int rank)
{
    int count = constants::kTestVecSize/size;
    double part_of_res = 0;
    double res = 0;

    double v1_buff[count];
    double v2_buff[constants::kTestVecSize];

    if (rank == constants::kSource)
    {
        auto vec1 = InitVector(constants::kTestVecSize);
        auto vec2 = InitVector(constants::kTestVecSize);

        for(int i = 1; i < size; ++i)
        {
            MPI_Send(&vec1[i*count], count, MPI_DOUBLE, i, constants::kTag, MPI_COMM_WORLD);
            MPI_Send(&vec2[0], constants::kTestVecSize, MPI_DOUBLE, i, constants::kTag, MPI_COMM_WORLD);
        }
        res += Algorithm(vec1, vec2, count);
    }
    else
    {
        MPI_Recv(&v1_buff, count, MPI_DOUBLE, constants::kSource, constants::kTag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&v2_buff, constants::kTestVecSize, MPI_DOUBLE, constants::kSource, constants::kTag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        part_of_res = Algorithm(v1_buff, v2_buff, count);
        MPI_Send(&part_of_res, 1, MPI_DOUBLE, constants::kSource, constants::kTag, MPI_COMM_WORLD);
    }

    if (rank == constants::kSource)
    {
        for (auto i = 1; i < size; ++i)
        {
            MPI_Recv(&part_of_res, 1, MPI_DOUBLE, i, constants::kTag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            res += part_of_res;
        }
    }
    return res;
}

int main(int argc, char* argv[])
{
    int rank, size;
    double max = 0, ret_val;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 0; i < constants::kTimes; ++i)
    {
        start = MPI_Wtime();
        ret_val = StartComputing(size, rank);
        end = MPI_Wtime() - start;
        if (end > max) { max = end; }
    }

    if (rank == constants::kSource)
    {
        std::cout << "Process: " << rank << " Computed value " << ret_val << std::endl;
        std::cout << "Elapsed time: " << max << std::endl;
    }

    MPI_Finalize();
    return 0;
}
