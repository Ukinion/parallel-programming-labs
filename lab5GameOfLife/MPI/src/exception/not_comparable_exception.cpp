#include "not_comparable_exception.h"

not_comparable_exception::not_comparable_exception(std::string message)
: game_exception(std::move(message))
{}