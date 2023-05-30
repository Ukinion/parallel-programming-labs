#include "game_exception.h"

#include <utility>

game_exception::game_exception(std::string  message)
: message_(std::move(message))
{}

const char* game_exception::what() const noexcept{
    return message_.c_str();
}
