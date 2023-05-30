#include "game-controller/launch_game_controller.h"


int main() {
    MPI_Init(nullptr, nullptr);
    LaunchController launch_controller;
    try {
        launch_controller.SetupGameSettings(120, 100);
        launch_controller.LaunchGame();
        launch_controller.WithdrawGame();
    } catch (const game_exception& e) {
        std::cerr << e.what() << std::endl;
    }
    MPI_Finalize();
    return 0;
}
