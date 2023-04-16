#ifndef NOMPI_CLASS_MATRIX_H
#define NOMPI_CLASS_MATRIX_H

#include <random>
#include <iostream>
#include <fstream>
#include <memory>
#include "xmmintrin.h"

using SymmetricMatrix = class matrix;
using Vector = class matrix;

constexpr int kPackSize = 2;
constexpr int kOddChecker = 0x1;
constexpr int kExitSuccess = 0;
constexpr int kExitFailure = 1;

class matrix
{
public:
    matrix(int, int, const std::string&);
    matrix(int, int, double);
    matrix(int,int, const std::vector<double>&);

    friend matrix operator+(const matrix&, const matrix&);
    friend matrix operator-(const matrix&, const matrix&);
    friend matrix operator*(const matrix&, const matrix&);
    friend double operator^(const matrix&, const matrix&);
    friend double operator!(const matrix&);
    friend matrix operator&(double, const matrix&);

    void Print() const;
    void PrintPart() const;
private:
    void ReadDataFromFile(const std::string&);

    std::vector<double> matrix_;
    int row_;
    int col_;
    int full_size_;
};



#endif
