#pragma once

#include "game_exception.h"

class invalid_option_exception: public game_exception {
public:
    explicit invalid_option_exception(std::string);
private:
    std::string message_;
};


