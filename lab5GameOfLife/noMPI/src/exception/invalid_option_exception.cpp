#include "invalid_option_exception.h"

#include <utility>

invalid_option_exception::invalid_option_exception(std::string message)
: game_exception(std::move(message)) {}