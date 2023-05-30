#include "launch_game_controller.h"

LaunchController::LaunchController() {
    game_ = std::make_unique<GameOfLife>();
}

void LaunchController::SetupGameSettings(int row, int col) {
    settings_.row = row;
    settings_.col = col;
    settings_.start_x = 1;
    settings_.start_y = 0;
    //RequestPatternStartPos();
}

void LaunchController::RequestPatternStartPos() {
    std::string coordinates;
    std::cout << "\n\t********************************" << std::endl;
    std::cout << "\tEnter start position [x] [y]: ";
    std::cin >> settings_.start_x >> settings_.start_y;
    std::cout << "\t********************************" << std::endl;
}

void LaunchController::LaunchGame() {
    try {
        game_->InitGameResource(settings_.row, settings_.col);
        game_->StartGame(settings_.start_x, settings_.start_y);
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
