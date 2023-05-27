#include "pthread_model.h"

int main(int argc, char* argv[]) {
    int provided, rank;
    double start, execution_time;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    if (provided != MPI_THREAD_MULTIPLE) {
        std::cerr << "Invalid thread level support\n";
        std::exit(constants::logic::MPI_INIT_FAILED);
    }
    start = MPI_Wtime();
    EmulationConcurrencyWork();
    execution_time = MPI_Wtime() - start;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == constants::process::PRINTER_PROCESS) {
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "\t***OVERALL***" << std::endl;
        std::cout << "execution time: " << execution_time << std::endl;
        std::cout << "------------------------------------------------------" << std::endl << std::endl;
    }
    MPI_Finalize();
    return 0;
}