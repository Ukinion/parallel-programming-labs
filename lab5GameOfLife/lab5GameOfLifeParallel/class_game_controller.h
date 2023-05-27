#ifndef LAB5GAMEOFLIFEPARALLEL_CLASS_GAME_CONTROLLER_H
#define LAB5GAMEOFLIFEPARALLEL_CLASS_GAME_CONTROLLER_H

#include "class_game_of_life.h"

class GameController {
public:
    GameController(int num_row ,int num_col) {
        game_ptr_ = new GameOfLife(num_row, num_col);
    }

    void StartGame() {
        game_ptr_->StartNewGame();
    }

    void EndGame() {
        game_ptr_->EndGame();
    }

    ~GameController() {
        delete game_ptr_;
    }

private:
    GameOfLife* game_ptr_;
};


#endif
