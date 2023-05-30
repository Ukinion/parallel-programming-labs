#pragma once

#include <exception>
#include <string>
#include <utility>

class game_exception: public std::exception {
public:
    explicit game_exception(std::string);
    const char* what() const noexcept override;
private:
    std::string message_;
};