#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

bool is_symbol(char c)
{
  return !std::isdigit(c) && (c != '.');
}

// check if a symbol is touching this position in a halo around the position
bool position_is_near_symbol(const std::vector<std::string> &data, size_t posx, size_t posy)
{
  char up = posx > 0 ? data[posx - 1][posy] : '.';
  char down = posx < data.size() - 1 ? data[posx + 1][posy] : '.';
  char left = posy > 0 ? data[posx][posy - 1] : '.';
  char right = posy < data[posx].size() - 1 ? data[posx][posy + 1] : '.';
  char diag_up_right = ((posx > 0) && (posy < data[posx].size() - 1)) ? data[posx - 1][posy + 1] : '.';
  char diag_up_left = ((posx > 0) && (posy > 0)) ? data[posx - 1][posy - 1] : '.';
  char diag_down_right = ((posx < data.size() - 1) && (posy < data[posx].size() - 1)) ? data[posx + 1][posy + 1] : '.';
  char diag_down_left = ((posx < data.size() - 1) && (posy > 0)) ? data[posx + 1][posy - 1] : '.';

  return is_symbol(up) || is_symbol(down) || is_symbol(left) || is_symbol(right) ||
         is_symbol(diag_up_right) || is_symbol(diag_up_left) || is_symbol(diag_down_right) || is_symbol(diag_down_left);
}

int count(const std::vector<std::string> &data)
{
  int sum = 0;
  for (size_t i = 0; i < data.size(); ++i)
  {
    size_t num_start = -1;
    bool keep = false;
    for (size_t j = 0; j < data[i].size(); ++j)
    {
      if (std::isdigit(data[i][j]))
      {
        if (num_start == -1)
        {
          num_start = j;
        }
        if (keep || position_is_near_symbol(data, i, j))
        {
          keep = true;
        }
      }
      else
      {
        if (num_start != -1 && keep)
        {
          sum += std::stoi(data[i].substr(num_start, j - num_start));
        }
        keep = false;
        num_start = -1;
      }
      // there is a number at the edge
      if ((j == data[i].size() - 1) && (num_start != -1 && keep))
      {
        sum += std::stoi(data[i].substr(num_start));
      }
    }
  }
  return sum;
}

int main()
{
  std::ifstream file(std::filesystem::path("inputs/day3.txt"));
  std::string line;
  std::vector<std::string> data;

  while (std::getline(file, line))
  {
    char edge = line[line.size() - 1];
    if (edge == '\r' || edge == '\n')
    {
      line.pop_back();
    }
    data.push_back(line);
  }

  int part1_sum = count(data);

  std::cout << "Part 1: " << part1_sum << std::endl;
}