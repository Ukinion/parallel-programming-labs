#ifndef LAB1TWOVECTORS_LINEARVECTOR_H
#define LAB1TWOVECTORS_LINEARVECTOR_H

#include <vector>
#include <random>
#include <mpi.h>

#include "Exceptions.h"

namespace constants
{
    constexpr auto kRandomRange = 20;
    constexpr auto kTimes = 5;
    constexpr auto kTestVecSize = 100000;
}

namespace lab1
{
    template <typename T>
    auto GetRandomGenerator(int range)
    { throw exception::UnsupportedVectorType(); }

    template <>
    auto GetRandomGenerator<int>(int range)
    { std::uniform_int_distribution<int> dist(-range, range); return dist; }

    template <>
    auto GetRandomGenerator<double>(int range)
    { std::uniform_real_distribution<double> dist(-range, range); return dist; }

    template <typename T>
    class LinearVector
    {
    public:
        LinearVector(long long size)
        {
            if (size < 0) { throw exception::UnsupportedVectorSize(); }

            size_= size;
            std::vector<T> rnd_vec (size);

            std::random_device rnd_dev;
            std::mt19937 eng(rnd_dev());
            auto dist = GetRandomGenerator<T>(constants::kRandomRange);

            for(size_t i = 0; i < size; ++i) { rnd_vec[i] = dist(eng); }
            vec_ = rnd_vec;
        }

        size_t GetVectorSize() const { return size_; }
        std::vector<T> GetVector() const { return vec_; }

        void PrintVector() const
        { for (const auto& i : vec_) { std::cout << i << " "; } }
    private:
        std::vector<T> vec_;
        std::size_t size_;
    };

    template <typename T>
    void Algorithm(const LinearVector<T>& vec1, const LinearVector<T>& vec2)
    {
        if (vec1.GetVectorSize() != vec2.GetVectorSize()) { throw exception::InvalidAlgorithmParameters(); }

        long long s = 0;
        std::vector<T> cp_vec1 = vec1.GetVector();
        std::vector<T> cp_vec2 = vec2.GetVector();
        double mx_time = 0;

        for (int k = 0; k < constants::kTimes; ++k)
        {
            auto start = MPI_Wtime();
            for (auto i = 0; i < constants::kTestVecSize; ++i) {
                for (auto j = 0; j < constants::kTestVecSize; ++j) { s += cp_vec1[i] * cp_vec2[j]; }
            }
            auto el_time = MPI_Wtime() - start;

            if (el_time > mx_time) { mx_time = el_time; }
        }

        std::cout << "Computed value " << s << std::endl;
        std::cout << "Time elapsed " << mx_time << " secs" << std::endl;
    }
}

#endif
