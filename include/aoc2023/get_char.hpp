#pragma once

#include <string>
#include <vector>

#include <aoc2023/position.hpp>

char get_char(const std::vector<std::string> &data, Pos cur, char unfound = '\0');
char get_char(const std::vector<std::string> &data, size_t i, size_t j, char unfound = '\0');