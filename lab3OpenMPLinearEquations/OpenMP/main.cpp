#include "sofe_algorithm_solution.h"
#include "omp.h"

void fa()
{
    int n = 10;
    #pragma omp for
    for (int i = 0; i < n; ++i)
    {
        #pragma omp critical
        {
            std::cout << omp_get_thread_num() << " " << i << " " << &i << " " << &n << std::endl;
        }
    }
}

int main(int argc, char* argv[])
{
    #pragma omp parallel num_threads(5) default(none)
    {
        fa();
    }
    //return LabNoMPI(argc, argv);
}
