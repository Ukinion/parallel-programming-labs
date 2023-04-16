#ifndef MPI_CLASS_MPI_MATRIX_H
#define MPI_CLASS_MPI_MATRIX_H

#include <iostream>
#include <random>
#include <cstring>
#include <fstream>

#include "mpi.h"
#include "xmmintrin.h"

constexpr int kRoot = 0;
constexpr int kPackSize = 2;
constexpr int kOddChecker = 0x1;

constexpr int kExitSuccess = 0;
constexpr int kExitFailure = 1;

using MPI_Vector = class MPI_Matrix;

class MPI_Matrix
{
public:
    class MPI_Options
    {
    public:
        MPI_Options() = default;
        MPI_Options(int, int);
        MPI_Options(const MPI_Options &);

        ~MPI_Options();

        MPI_Options& operator=(const MPI_Options&);
        void SetupOptions(int, int);

        int size_;
        int rank_;
        int local_start_;
        int local_end_;

        int* displs_;
        int* cnts_per_proc_;
        int* rows_per_proc_;
    private:
        void CopyMemory(const MPI_Options&);
    };

    MPI_Matrix(int, int, double);
    MPI_Matrix(int, int);
    MPI_Matrix(int, int, int, const std::vector<double>&, const MPI_Options&);
    MPI_Matrix(const MPI_Matrix&);

    ~MPI_Matrix();

    MPI_Matrix& operator=(const MPI_Matrix&);

    void ReadDataFromFile(const std::string&);
    void TranslateSlicedMatrix();
    void TranslateVector();
    void PrintPart() const;

    friend MPI_Matrix operator+(const MPI_Matrix&, const MPI_Matrix&);
    friend MPI_Matrix operator-(const MPI_Matrix&, const MPI_Matrix&);
    friend MPI_Matrix operator*(const MPI_Matrix&, const MPI_Vector&);
    friend MPI_Matrix operator&(double, const MPI_Matrix&);

    friend double operator^(const MPI_Vector&, const MPI_Vector&);
    friend double operator!(const MPI_Vector&);
private:
    void InitLocalPart();
    void ConfigureOptions();

    friend MPI_Matrix BuildMatrix(std::vector<double>&, const MPI_Matrix&);
    static double BuildData(double);

    std::vector<double> matrix_;
    int row_;
    int col_;
    int full_size_;

    MPI_Options* options_ = nullptr;
    MPI_Matrix* local_matrix_ = nullptr;
};

#endif

