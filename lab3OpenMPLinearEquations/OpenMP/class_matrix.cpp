#include "class_matrix.h"

void matrix::ReadDataFromFile(const std::string& data_file_name)
{
    std::fstream data_storage (data_file_name, std::ios::in);

    if (data_storage.is_open())
    {
        int rcount = 0;
        std::string buff (10, 'c');

        while (!data_storage.eof())
        {
            std::getline(data_storage, buff,' ');
            matrix_[rcount++] = std::stod(buff);
        }
    }
    else
    {
        std::cerr << "File is not exist!";
        exit(kExitFailure);
    }
}

matrix::matrix(int row, int col, const std::string& mt_data_file_name) :
row_ (row), col_ (col), full_size_ (row*col), matrix_ (row*col)
{
    ReadDataFromFile(mt_data_file_name);
}

matrix::matrix(int row, int col, double val) :
row_ (row), col_ (col), full_size_ (row*col), matrix_(row*col, val) {}

matrix::matrix(int row, int col, const std::vector<double>& mat) :
row_ (row), col_ (col), full_size_ (row*col), matrix_ (mat) {}

void matrix::Print() const
{
    for (auto i = 0; i < row_; ++i)
    {
        for (auto j = 0; j < col_; ++j)
        {
            std::cout << matrix_[i*col_+j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void matrix::PrintSolutionPart() const
{
    std::cout << "Vector-solution parts: ";
    std::cout << matrix_[0] << " ";
    std::cout << matrix_[row_] << " ";
    std::cout << matrix_[col_] << " ";
    std::cout << matrix_[full_size_-1] << std::endl << std::endl;
}

matrix operator+(const matrix& mat1, const matrix& mat2)
{
    __m128d part_mat1, part_mat2, db_op_res;
    int loop_end = mat1.full_size_;
    bool is_odd = false;
    std::vector<double> result_mat (mat1.full_size_);

    if (loop_end & kOddChecker)
    {
        is_odd = true;
        loop_end -= 2;
    }

    for (auto i = 0; i < loop_end; i+=2)
    {
        part_mat1 = _mm_loadu_pd(&mat1.matrix_[i]);
        part_mat2 = _mm_loadu_pd(&mat2.matrix_[i]);
        db_op_res = _mm_add_pd(part_mat1, part_mat2);
        _mm_storeu_pd(&result_mat[i], db_op_res);
    }

    if (is_odd)
    {
        result_mat[loop_end+1] = mat1.matrix_[loop_end+1] + mat2.matrix_[loop_end+1];
    }

    return {mat1.row_, mat2.col_, result_mat };
}

matrix operator-(const matrix& mat1, const matrix& mat2)
{
    __m128d part_mat1, part_mat2, db_op_res;
    int loop_end = mat1.full_size_;
    bool is_odd = false;
    std::vector<double> result_mat (mat1.full_size_);

    if (loop_end & kOddChecker)
    {
        is_odd = true;
        loop_end -= 2;
    }

    for (auto i = 0; i < loop_end; i+=2)
    {
        part_mat1 = _mm_loadu_pd(&mat1.matrix_[i]);
        part_mat2 = _mm_loadu_pd(&mat2.matrix_[i]);
        db_op_res = _mm_sub_pd(part_mat1, part_mat2);
        _mm_storeu_pd(&result_mat[i], db_op_res);
    }

    if (is_odd)
    {
        result_mat[loop_end+1] = mat1.matrix_[loop_end+1] - mat2.matrix_[loop_end+1];
    }

    return {mat1.row_, mat2.col_, result_mat };
}

matrix operator*(const matrix& mat1, const matrix & mat2)
{
    __m128d part_row_mat1, part_row_mat2, inter_val;
    __m128d halves_part = _mm_set_sd(0);
    int loop_end = mat1.col_;
    bool is_odd = false;
    std::vector<double> result_mat (mat1.row_ * mat2.col_, 0);
    auto stg = new double[2];

    if (loop_end & kOddChecker)
    {
        is_odd = true;
        loop_end -= 2;
    }

    for (auto k = 0; k < mat2.col_; ++k)
    {
        for (auto i = 0; i < mat1.row_; ++i)
        {
            for (auto j = 0; j < loop_end; j+=2)
            {
                part_row_mat1 = _mm_loadu_pd(&mat1.matrix_[mat1.col_*i+j]);
                part_row_mat2 = _mm_loadu_pd(&mat2.matrix_[j]);
                inter_val = _mm_mul_pd(part_row_mat1, part_row_mat2);
                halves_part = _mm_add_pd(inter_val, halves_part);
            }
            _mm_storeu_pd(stg, halves_part);
            halves_part = _mm_set_sd(0);
            result_mat[i] = stg[0] + stg[1];

            if (is_odd)
            {
                result_mat[i] += mat1.matrix_[mat1.col_*i+loop_end+1] * mat2.matrix_[loop_end+1];
            }
        }
    }

    delete[] stg;
    return {mat1.row_, mat2.col_, result_mat };
}

double operator^(const Vector& vec1, const Vector& vec2)
{
    __m128d part_vec1, part_vec2, dt_inter_pair;
    __m128d halves_prod = _mm_set_sd(0);
    int loop_end = vec1.full_size_;
    bool is_odd = false;
    double dt_prod = 0;
    auto stg = new double[kPackSize];

    if (loop_end & kOddChecker)
    {
        is_odd = true;
        loop_end -= 2;
    }

    for (auto i = 0; i < loop_end; i+=2)
    {
        part_vec1 = _mm_loadu_pd(&vec1.matrix_[i]);
        part_vec2 = _mm_loadu_pd(&vec1.matrix_[i]);
        dt_inter_pair = _mm_mul_pd(part_vec1, part_vec2);
        halves_prod = _mm_add_pd(halves_prod, dt_inter_pair);
    }

    _mm_storeu_pd(stg, halves_prod);
    dt_prod += stg[0] + stg[1];

    if (is_odd)
    {
        dt_prod += vec1.matrix_[loop_end+1] * vec2.matrix_[loop_end+1];
    }

    delete[] stg;
    return dt_prod;
}

double operator!(const Vector& vec)
{
    __m128d part_vec, rpt_part, part_norm;
    __m128d halves_prod = _mm_set_sd(0);
    int loop_end = vec.full_size_;
    double norm = 0;
    bool is_odd = false;
    auto stg = new double[kPackSize];

    if (loop_end & kOddChecker)
    {
        is_odd = true;
        loop_end -= 2;
    }

    for (auto i = 0; i < loop_end; i+=2)
    {
        part_vec = _mm_loadu_pd(&vec.matrix_[i]);
        rpt_part = _mm_loadu_pd(&vec.matrix_[i]);
        part_norm = _mm_mul_pd(part_vec, rpt_part);
        halves_prod = _mm_add_pd(halves_prod, part_norm);
    }

    _mm_storeu_pd(stg, halves_prod);
    norm += stg[0] + stg[1];

    if (is_odd)
    {
        norm += vec.matrix_[loop_end+1] * vec.matrix_[loop_end+1];
    }

    delete[] stg;
    return norm;
}

matrix operator&(double scale, const matrix& mat)
{
    __m128d part_mat, scaled_parts;
    __m128d scale_v = _mm_set1_pd(scale);
    int loop_end = mat.full_size_;
    bool is_odd = false;
    std::vector<double> scaled_mat (mat.full_size_);

    if (loop_end & kOddChecker)
    {
        is_odd = true;
        loop_end -= 2;
    }

    for (auto i = 0; i < loop_end; i+=2)
    {
        part_mat = _mm_loadu_pd(&mat.matrix_[i]);
        scaled_parts = _mm_mul_pd(part_mat, scale_v);
        _mm_storeu_pd(&scaled_mat[i], scaled_parts);
    }

    if (is_odd)
    {
        scaled_mat[loop_end + 1] = scale * mat.matrix_[loop_end+1];
    }

    return {mat.row_, mat.col_, scaled_mat };
}

