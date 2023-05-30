#include "out_of_exception.h"

out_of_exception::out_of_exception(std::string message)
: game_exception(std::move(message))
{}