#include <iostream>
#include <vector>
#include <random>
#include <mpi.h>

namespace constants
{
    constexpr auto kRandomRange = 20;
    constexpr auto kTestVecSize = 90000;
    constexpr auto kSource = 0;
    constexpr auto kTimes = 5;
}

void InitVector(double* vector)
{
    std::random_device rnd_dev;
    std::mt19937 eng(rnd_dev());
    std::uniform_real_distribution<double> dist(-constants::kRandomRange, constants::kRandomRange);

    for(size_t i = 0; i < constants::kTestVecSize; ++i) { vector[i] = dist(eng); }
}

double Algorithm(const double* vec1, const double* vec2, int v1_size)
{
    double s = 0;

    for (auto i = 0; i < v1_size; ++i)
    {
        for (auto j = 0; j < constants::kTestVecSize; ++j) { s += vec1[i] * vec2[j]; }
    }
    return s;
}

double StartComputing(int rank, int size)
{
    int count = constants::kTestVecSize/size;
    double ret_s = 0, part_s;

    double recv_buff[count];
    auto vec1 = new double[constants::kTestVecSize];
    auto vec2 = new double[constants::kTestVecSize];

    if (rank == constants::kSource)
    {
        InitVector(vec1);
        InitVector(vec2);
    }
    MPI_Scatter(vec1, count, MPI_DOUBLE, recv_buff,
                count, MPI_DOUBLE, constants::kSource, MPI_COMM_WORLD);
    MPI_Bcast(vec2, constants::kTestVecSize, MPI_DOUBLE, constants::kSource, MPI_COMM_WORLD);

    part_s = Algorithm(recv_buff, vec2, count);
    MPI_Reduce(&part_s, &ret_s, 1, MPI_DOUBLE, MPI_SUM, , MPI_COMM_WORLD);

    delete[] vec1;
    delete[] vec2;
    return ret_s;
}

int main(int argc, char* argv[])
{

    int rank, size;
    double start, el_tm, ret_v;
    double mx_tm = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 0; i < constants::kTimes; ++i)
    {
        start = MPI_Wtime();
        ret_v = StartComputing(rank, size);
        el_tm = MPI_Wtime() - start;
        if (el_tm > mx_tm) { mx_tm = el_tm; }
    }

    if (rank == constants::kSource)
    {
        std::cout << "Process: " << rank << " Computed val: " << ret_v << std::endl;
        std::cout << "Elapsed time: " << mx_tm << std::endl;
    }

    MPI_Finalize();
    return 0;
}
