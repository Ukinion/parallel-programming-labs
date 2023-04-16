#include "sofe_algorithm_solution.h"

constexpr double kEpsilon = 1e-10;
constexpr double kInfinity = 1e+10;
constexpr double kSolutionFound = 1e+100;
constexpr int kMaxIterCount = 50000;
constexpr int kMinMatrixSize = 0;
constexpr int kMinStreakOfSolution = 30;
constexpr int kTimes = 6;

double GetNextAlpha(double r_n_dt_prod, const SymmetricMatrix& A, const Vector& z_n)
{
    auto dt_prod = ((A * z_n) ^ z_n);
    if (dt_prod == 0) { return kSolutionFound; }

    return (r_n_dt_prod / dt_prod);
}

Vector GetNextVectorX(const Vector& x_n, double alpha_n_next, const Vector& z_n)
{
    return (x_n + (alpha_n_next & z_n));
}

Vector GetNextVectorR(const Vector& r_n, double alpha_n_next, const SymmetricMatrix& A, const Vector& z_n)
{
    return (r_n - (alpha_n_next & (A * z_n)));
}

double GetNextBeta(double r_n_dt_prod, const Vector& r_n_next)
{
    if (r_n_dt_prod == 0) { return kSolutionFound; }
    return (!r_n_next) / r_n_dt_prod;
}

Vector GetNextVectorZ(const Vector& r_n_next, double beta_n_next, const Vector& z_n)
{
    return (r_n_next - (beta_n_next & z_n));
} scatv_tm

Vector ConjugatedGradientMethod(const SymmetricMatrix& A, const Vector& x_0, const Vector& b)
{
    Vector r_n = (b - A * x_0);
    Vector z_n = r_n;
    Vector x_n = x_0;

    double r_n_norm_square;
    double alpha, beta;
    double end_point = kEpsilon * !b;

    int iter_count = 0;
    int streak = 0;

    while (iter_count++ < kMaxIterCount)
    {
        if ((r_n_norm_square = !r_n)  < end_point)
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

int LabNoMPI(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    double start;
    double end;
    double min_tm = kInfinity;

    int N = std::stoi(argv[1]);
    if (N <= kMinMatrixSize)
    {
        MPI_Finalize();
        std::cerr << "Bad size";
        return kExitFailure;
    }

    std::string mat_A_file (argv[2]);
    std::string vec_b_file (argv[3]);

    auto A = new SymmetricMatrix(N, N, mat_A_file);
    auto b = new Vector(N, 1, vec_b_file);
    auto x = new Vector(N, 1, 0.0);

    start = MPI_Wtime();
    for (auto i = 1; i < kTimes; ++i)
    {
        *x = ConjugatedGradientMethod(*A, *x, *b);
        end = MPI_Wtime() - start;
        if (end < min_tm) { min_tm = end; }
        start = MPI_Wtime();
    }

    b->PrintPart();
    (*A * *x).PrintPart();
    std::cout << "Processes: " << 1 << std::endl;
    std:: cout << "Computing time established: " << min_tm << std::endl;

    delete A;
    delete b;
    delete x;
    MPI_Finalize();

    return kExitSuccess;
}
