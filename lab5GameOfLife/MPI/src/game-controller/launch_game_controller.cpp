#include "launch_game_controller.h"

LaunchController::LaunchController() {
    game_ = std::make_unique<GameOfLife>();
}

void LaunchController::PrepareGameResources() {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int* send_counts;
    int* displacement;
    int* rows;
    GameField game_field;
    if (rank == constants::game::ROOT) {
        game_field.InitGameField(settings_.row, settings_.col);
        game_field.CreateGlider(settings_.start_x, settings_.start_y);
    }
    CountDataForScatter(send_counts, displacement, rows, size);
    std::vector<char> field_part (send_counts[rank]);
    MPI_Scatterv(&game_field.GetField()[0], send_counts, displacement,
                 MPI_CHAR, &field_part[0], send_counts[rank],
                 MPI_CHAR, constants::game::ROOT, MPI_COMM_WORLD);
    field_part_.SetNewField(field_part, rows[rank], settings_.col, rank, size);
    settings_.row = rows[rank];
    delete[] send_counts;
    delete[] displacement;
    delete[] rows;
}

void LaunchController::CountDataForScatter(int* send_counts, int* displacement, int* rows, int size) {
    int rows_per_proc = settings_.row / size;
    int remain_rows = settings_.row - rows_per_proc * size;
    int offset = 0;
    send_counts = new int[size];
    rows = new int[size];
    displacement = new int[size]{};
    for (auto i = 0; i < size; ++i) {
        if (remain_rows) {
            send_counts[i] = (rows_per_proc + 1) * settings_.col;
            rows[i] = rows_per_proc + 1;
            remain_rows--;
        } else {
            send_counts[i] = rows_per_proc * settings_.col;
            rows[i] = rows_per_proc;
        }
        displacement[i] += offset;
        offset += send_counts[i];
    }
}

void LaunchController::SetupGameSettings(int row, int col) {
    settings_.row = row;
    settings_.col = col;
    settings_.start_x = 1;
    settings_.start_y = 0;
}

void LaunchController::LaunchGame() {
    try {
        game_->SetField(field_part_);
        game_->StartGame();
    } catch (const game_exception& e) {
        std::cerr << "\n\t*******************************";
        std::cerr << e.what() << std::endl;
        std::cout << "\tInternal Error!\n\tAbort game..." << std::endl;
        std::cerr << "\t*******************************" << std::endl;
        std::exit(constants::runtime::EXCEPTION_THROW);
    }
}

void LaunchController::WithdrawGame() {
    game_->EndGame();
}
