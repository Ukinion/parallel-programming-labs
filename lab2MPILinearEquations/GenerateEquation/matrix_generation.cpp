#include "matrix_generation.h"

void GenerateSymmetricMatrixFile(int size, int load, int rnd_range)
{
    std::ofstream matrix_file ("Matrix_A.txt", std::ios::out);
    matrix_file.precision(2);

    std::vector<double> inter_mat (size*size);

    std::random_device dev;
    std::mt19937 eng(dev());
    std::uniform_real_distribution<double> dist(-rnd_range, rnd_range);

    for (auto i = 0; i < size; ++i)
    {
        for (auto j = i; j < size; ++j)
        {
            if (i == j)
            {
                inter_mat[i*size+j] = dist(eng) + load;
            }
            else
            {
                inter_mat[i*size+j] = dist(eng);
                inter_mat[j*size+i] = inter_mat[i*size+j];
            }
        }
    }

    for (auto i = 0; i < size-1; ++i)
    {
        for (auto j = 0; j < size; ++j)
        {
            matrix_file << inter_mat[i*size+j] << ' ';
        }
    }

    for (auto j = 0; j < size-1; ++j)
    {
        matrix_file << inter_mat[(size-1)*size+j] << ' ';
    }
    matrix_file << inter_mat[size*(size-1)+size-1];
}

void GenerateVectorFile(int size, int rnd_range)
{
    std::ofstream vector_file ("Vector_b.txt", std::ios::out);
    vector_file.precision(2);

    std::random_device dev;
    std::mt19937 eng(dev());
    std::uniform_real_distribution<double> dist(-rnd_range, rnd_range);

    for (auto i = 0; i < size-1; ++i)
    {
        vector_file << dist(eng) << ' ';
    }
    vector_file << dist(eng);
}