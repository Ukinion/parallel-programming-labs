#include <iostream>
#include "class_game_controller.h"
#include "mpi.h"

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    GameController game_controller(std::stoi(argv[1]), std::stoi(argv[2]));
    double ref_tm, game_tm, min_tm = constants::runtime::INFINITY;

    for (auto i = 0; i < constants::runtime::SAMPLE; ++i)
    {
        ref_tm = MPI_Wtime();
        
        game_controller.StartGame();

        game_tm = MPI_Wtime() - ref_tm;
        game_controller.EndGame();
        if (game_tm < min_tm)
        {
            min_tm = game_tm;
        }
    }
    std::cout << "game time: " << min_tm << std::endl;
    MPI_Finalize();

    return 0;
}
