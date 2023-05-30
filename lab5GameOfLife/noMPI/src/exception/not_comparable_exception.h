#pragma once

#include "game_exception.h"

class not_comparable_exception: public game_exception {
public:
    explicit not_comparable_exception(std::string);
private:
    std::string message_;
};