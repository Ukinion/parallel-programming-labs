#include "class_mpi_matrix.h"

void MPI_Matrix::ReadDataFromFile(const std::string& data_file_name)
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

void MPI_Matrix::ConfigureOptions()
{
    options_ = new MPI_Options(row_, col_);
}

MPI_Matrix::MPI_Matrix(int row, int col) :
row_ (row), col_ (col), full_size_ (row*col), matrix_(row*col)
{
    ConfigureOptions();
}

MPI_Matrix::MPI_Matrix(int row, int col, double val) :
row_ (row), col_ (col), full_size_(col*row), matrix_(col*row, val)
{
    ConfigureOptions();
}

MPI_Matrix::MPI_Matrix(int row, int col, int fullsize, const std::vector<double>& vec, const MPI_Options& options) :
row_ (row), col_ (col), full_size_(fullsize), matrix_(vec)
{
    options_ = new MPI_Options(options);
}

MPI_Matrix::MPI_Matrix(const MPI_Matrix& mat) :
row_ (mat.row_), col_ (mat.col_), full_size_ (mat.full_size_), matrix_ (mat.matrix_)
{
    options_ = new MPI_Options(*mat.options_);
}

void MPI_Matrix::InitLocalPart()
{
    local_matrix_ = new MPI_Matrix(options_->rows_per_proc_[options_->rank_], col_, 0);
}

void MPI_Matrix::TranslateSlicedMatrix()
{
    InitLocalPart();
    MPI_Scatterv(&matrix_[0], options_->cnts_per_proc_,
                 options_->displs_,MPI_DOUBLE,
                 &local_matrix_->matrix_[0], options_->cnts_per_proc_[options_->rank_],
                 MPI_DOUBLE, kRoot ,MPI_COMM_WORLD);
}

void MPI_Matrix::TranslateVector()
{
    MPI_Bcast(&matrix_[0], full_size_, MPI_DOUBLE, kRoot, MPI_COMM_WORLD);
}

void MPI_Matrix::PrintPart() const
{
   std::cout << "Vector parts: ";
   std::cout << matrix_[0] << " ";
   std::cout << matrix_[col_] << " ";
   std::cout << matrix_[row_-1] << " ";
   std::cout << matrix_[full_size_-1] << std::endl << std::endl;
}

MPI_Matrix BuildMatrix(std::vector<double>& vec, const MPI_Matrix& mat)
{
    MPI_Allgatherv(&vec[0]+mat.options_->local_start_,
                   mat.options_->cnts_per_proc_[mat.options_->rank_], MPI_DOUBLE,
                   &vec[0], mat.options_->cnts_per_proc_, mat.options_->displs_,
                   MPI_DOUBLE, MPI_COMM_WORLD);
    return { mat.row_, mat.col_, mat.full_size_, vec, *mat.options_ };
}

double MPI_Matrix::BuildData(double part_data_proc)
{
    MPI_Allreduce(&part_data_proc, &part_data_proc, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return part_data_proc;
}

MPI_Matrix::~MPI_Matrix()
{
    delete options_;
    delete local_matrix_;
}

MPI_Matrix& MPI_Matrix::operator=(const MPI_Matrix& mat)
{
    if (this != &mat)
    {
    matrix_ = mat.matrix_;
    row_ = mat.row_;
    col_ = mat.col_;
    full_size_ = mat.full_size_;
    *options_ = *mat.options_;
    }
    return *this;
}

MPI_Matrix operator+(const MPI_Matrix& mat1, const MPI_Matrix& mat2)
{
    __m128d part_mat1, part_mat2, result_part;
    int loop_end = mat1.options_->local_end_;
    bool is_odd = false;
    std::vector<double> result_mat (mat1.full_size_);

    if (mat1.options_->cnts_per_proc_[mat1.options_->rank_] % kPackSize)
    {
        is_odd = true;
        loop_end -= 2;
    }

    for (auto i = mat1.options_->local_start_; i < loop_end; i+=2)
    {
        part_mat1 = _mm_loadu_pd(&mat1.matrix_[i]);
        part_mat2 = _mm_loadu_pd(&mat2.matrix_[i]);
        result_part = _mm_add_pd(part_mat1, part_mat2);
        _mm_storeu_pd(&result_mat[i], result_part);
    }

    if (is_odd)
    {
        result_mat[loop_end + 1] = mat1.matrix_[loop_end + 1] + mat2.matrix_[loop_end + 1];
    }

    return BuildMatrix(result_mat, mat1);
}

MPI_Matrix operator-(const MPI_Matrix& mat1, const MPI_Matrix& mat2)
{
    __m128d part_mat1, part_mat2, result_part;
    int loop_end = mat1.options_->local_end_;
    bool is_odd = false;
    std::vector<double> result_mat (mat1.full_size_);

    if (mat1.options_->cnts_per_proc_[mat1.options_->rank_] % kPackSize)
    {
        is_odd = true;
        loop_end -= 2;
    }

    for (auto i = mat1.options_->local_start_; i < loop_end; i+=2)
    {
        part_mat1 = _mm_loadu_pd(&mat1.matrix_[i]);
        part_mat2 = _mm_loadu_pd(&mat2.matrix_[i]);
        result_part = _mm_sub_pd(part_mat1, part_mat2);
        _mm_storeu_pd(&result_mat[i], result_part);
    }

    if (is_odd)
    {
        result_mat[loop_end + 1] = mat1.matrix_[loop_end + 1] - mat2.matrix_[loop_end + 1];
    }

    return BuildMatrix(result_mat, mat1);
}

MPI_Matrix operator*(const MPI_Matrix& mat, const MPI_Vector& vec)
{
    __m128d part_row_mat, part_row_vec, inter_val;
    __m128d halves_part = _mm_set_sd(0);
    int loop_end = mat.local_matrix_->col_;
    int global_idx = mat.options_->local_start_/mat.row_;
    bool is_odd = false;
    std::vector<double> result_mat (mat.row_);
    auto stg = new double[2];

    if (loop_end & kOddChecker)
    {
        is_odd = true;
        loop_end -= 2;
    }

    for (auto k = 0; k < vec.col_; ++k)
    {
        for (auto i = 0; i < mat.local_matrix_->row_; ++i, ++global_idx)
        {
            for (auto j = 0; j < loop_end; j+=2)
            {
                part_row_mat = _mm_loadu_pd(&mat.local_matrix_->matrix_[mat.col_*i+j]);
                part_row_vec = _mm_loadu_pd(&vec.matrix_[j]);
                inter_val = _mm_mul_pd(part_row_mat, part_row_vec);
                halves_part = _mm_add_pd(inter_val, halves_part);
            }
            _mm_storeu_pd(stg, halves_part);
            halves_part = _mm_set_sd(0);
            result_mat[global_idx] = stg[0] + stg[1];

            if (is_odd)
            {
                result_mat[global_idx] += mat.local_matrix_->matrix_[mat.col_*i+loop_end+1] * vec.matrix_[loop_end+1];
            }
        }
    }

    delete[] stg;
    return BuildMatrix(result_mat, vec);
}

double operator^(const MPI_Vector& vec1, const MPI_Vector& vec2)
{

    __m128d part_vec1, part_vec2, dt_inter_pair;
    __m128d halves_prod = _mm_set_sd(0);
    int loop_end = vec1.options_->local_end_;
    bool is_odd = false;
    double dt_prod = 0;
    auto stg = new double[2];

    if (vec1.options_->cnts_per_proc_[vec1.options_->rank_] & kOddChecker)
    {
        is_odd = true;
        loop_end -=2;
    }

    for (auto i = vec1.options_->local_start_; i < loop_end; i+=2)
    {
        part_vec1 = _mm_loadu_pd(&vec1.matrix_[i]);
        part_vec2 = _mm_loadu_pd(&vec2.matrix_[i]);
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
    return MPI_Matrix::BuildData(dt_prod);
}

double operator!(const MPI_Vector& vec)
{
    __m128d part_vec1, rpt_part, part_norm;
    __m128d halves_prod = _mm_set_sd(0);
    int loop_end = vec.options_->local_end_;
    bool is_odd = false;
    double sqr_norm = 0;
    auto stg = new double[2];

    if (vec.options_->cnts_per_proc_[vec.options_->rank_] & kOddChecker)
    {
        is_odd = true;
        loop_end -= 2;
    }

    for (auto i = vec.options_->local_start_; i < loop_end; i+=2)
    {
        part_vec1 = _mm_loadu_pd(&vec.matrix_[i]);
        rpt_part = _mm_loadu_pd(&vec.matrix_[i]);
        part_norm = _mm_mul_pd(part_vec1, rpt_part);
        halves_prod = _mm_add_pd(halves_prod, part_norm);
    }
    _mm_storeu_pd(stg, halves_prod);
    sqr_norm += stg[0] + stg[1];

    if (is_odd)
    {
        sqr_norm += vec.matrix_[loop_end + 1] * vec.matrix_[loop_end + 1];
    }

    delete[] stg;
    return MPI_Matrix::BuildData(sqr_norm);
}

MPI_Matrix operator&(double scale, const MPI_Matrix& mat)
{
    __m128d part_mat, scaled_parts;
    __m128d scale_v = _mm_set1_pd(scale);
    int loop_end = mat.options_->local_end_;
    bool is_odd = false;
    std::vector<double> scaled_mat (mat.full_size_);

    if (mat.options_->cnts_per_proc_[mat.options_->rank_] & kOddChecker)
    {
        is_odd = true;
        loop_end -= 2;
    }

    for (auto i = mat.options_->local_start_; i < loop_end; i+=2)
    {
        part_mat = _mm_loadu_pd(&mat.matrix_[i]);
        scaled_parts = _mm_mul_pd(part_mat, scale_v);
        _mm_storeu_pd(&scaled_mat[i], scaled_parts);
    }

    if (is_odd)
    {
        scaled_mat[loop_end + 1] = scale * mat.matrix_[loop_end+1];
    }

    return BuildMatrix(scaled_mat, mat);
}







