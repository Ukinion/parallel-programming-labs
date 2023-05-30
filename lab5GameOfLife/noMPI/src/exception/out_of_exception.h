#pragma once

#include "game_exception.h"

class out_of_exception: public game_exception {
public:
    explicit out_of_exception(std::string);
private:
    std::string message_;
};