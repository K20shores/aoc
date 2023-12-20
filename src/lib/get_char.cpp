#include <aoc2023/get_char.hpp>


char get_char(const std::vector<std::string> &data, Pos cur, char unfound) {
  if (cur.i >= 0 && cur.i < data.size() && cur.j >= 0 && cur.j < data[cur.i].size()) return data[cur.i][cur.j];
  return unfound;
}

char get_char(const std::vector<std::string> &data, size_t i, size_t j, char unfound) {
  if (i >= 0 && i < data.size() && j >= 0 && j < data[i].size()) return data[i][j];
  return unfound;
}