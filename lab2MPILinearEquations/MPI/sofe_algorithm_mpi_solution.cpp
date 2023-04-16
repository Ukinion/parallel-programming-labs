#include "sofe_algorithm_mpi_solution.h"

constexpr double kEpsilon = 1e-10;
constexpr double kInfinity = 1e+10;
constexpr int kMaxIterCount = 50000;
constexpr int kMinMatrixSize = 0;
constexpr int kMinStreakOfSolution = 30;
constexpr int kTimes = 6;
constexpr double kSolutionFound = 1e+100;

void PrepareSystem(int rank, char* argv[], MPI_Matrix& A, MPI_Vector& b)
{
    std::string mat_file_name (argv[2]);
    std::string vec_file_name (argv[3]);

    if (rank == kRoot)
    {
        A.ReadDataFromFile(mat_file_name);
        b.ReadDataFromFile(vec_file_name);
    }

    A.TranslateSlicedMatrix();
    b.TranslateVector();
}

double GetNextAlpha(double r_n_dt_prod, const MPI_Matrix& A, const MPI_Vector & z_n)
{
    auto dt_prod = ((A * z_n) ^ z_n);
    if (dt_prod == 0) { return kSolutionFound; }

    return (r_n_dt_prod / dt_prod);
}

MPI_Vector GetNextVectorX(const MPI_Vector& x_n, double alpha_n_next, const MPI_Vector& z_n)
{
    return (x_n + (alpha_n_next & z_n));
}

MPI_Vector GetNextVectorR(const MPI_Vector& r_n, double alpha_n_next, const MPI_Matrix& A, const MPI_Vector& z_n)
{
    return (r_n - (alpha_n_next & (A * z_n)));
}

double GetNextBeta(double r_n_dt_prod, const MPI_Vector& r_n_next)
{
    if (r_n_dt_prod == 0) { return kSolutionFound; }
    return (!r_n_next) / r_n_dt_prod;
}

MPI_Vector GetNextVectorZ(const MPI_Vector& r_n_next, double beta_n_next, const MPI_Vector& z_n)
{
    return (r_n_next - (beta_n_next & z_n));
}

MPI_Vector ConjugatedGradientMethod(const MPI_Matrix& A, const MPI_Vector& x_0, const MPI_Vector& b)
{
    MPI_Vector r_n = (b - A * x_0);
    MPI_Vector z_n = r_n;
    MPI_Vector x_n = x_0;

    double r_n_norm_square;
    double alpha, beta;
    double end_point = kEpsilon * !b;

    int iter_count = 0;
    int streak = 0;

    while (iter_count++ < kMaxIterCount)
    {
        if ((r_n_norm_square = !r_n) < end_point)
        {
            if (streak++ > kMinStreakOfSolution) { break; }
        }
        else { streak = 0; }

        alpha = GetNextAlpha(r_n_norm_square, A, z_n);
        if (alpha == kSolutionFound) { break; }

        x_n = GetNextVectorX(x_n, alpha, z_n);
        r_n = GetNextVectorR(r_n, alpha, A, z_n);

        beta = GetNextBeta(r_n_norm_square, r_n);
        if (beta == kSolutionFound) { break; }

        z_n = GetNextVectorZ(r_n, beta, z_n);
    }

    if (iter_count >= kMaxIterCount)
    {
        std::cout << "Solution diverged\nStop computing..." << std::endl;
        std::exit(kExitFailure);
    }

    return x_n;
}

int LabMPI(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start;
    double end;
    double min_tm = kInfinity;

    int N = std::stoi(argv[1]);
    if (N <= kMinMatrixSize)
    {
        MPI_Finalize();
        return kExitFailure;
    }

    auto A = new MPI_Matrix(N, N);
    auto b = new MPI_Vector(N, 1);
    auto x = new MPI_Vector(N, 1, -100.0);

    PrepareSystem(rank, argv, *A, *b);

    start = MPI_Wtime();
    for (auto i = 1; i < kTimes; ++i)
    {
        *x = ConjugatedGradientMethod(*A, *x, *b);
        end = MPI_Wtime() - start;
        if (end < min_tm) { min_tm = end; }
        start = MPI_Wtime();
    }

    auto s = (*A * *x);
    if (rank == kRoot)
    {
        b->PrintPart();
        s.PrintPart();
        std::cout << "Processes: " << size << std::endl;
        std::cout << "Computing time established: " << min_tm << std::endl;
    }

    delete A;
    delete b;
    delete x;
    MPI_Finalize();

    return kExitSuccess;
}