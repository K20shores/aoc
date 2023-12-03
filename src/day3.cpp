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
char get_char(const std::vector<std::string> &data, size_t x, size_t y)
{
  if (x >= 0 && x < data.size() && y >= 0 && y < data[x].size())
    return data[x][y];
  return '.';
}

bool position_is_near_symbol(const std::vector<std::string> &data, size_t x, size_t y)
{
  char neighbors[8] = {
      get_char(data, x - 1, y),
      get_char(data, x + 1, y),
      get_char(data, x, y - 1),
      get_char(data, x, y + 1),
      get_char(data, x - 1, y + 1),
      get_char(data, x - 1, y - 1),
      get_char(data, x + 1, y + 1),
      get_char(data, x + 1, y - 1)};

  for (char neighbor : neighbors)
  {
    if (is_symbol(neighbor))
      return true;
  }
  return false;
}

// add up all numbers that are adjacent to a symbol, a symbol is antyhing that's not a number or a '.'
int count_part1(const std::vector<std::string> &data)
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

// parse a number at a position
int get_number(const std::vector<std::string> &data, size_t x, size_t y)
{
  size_t left = 0;
  size_t right = 0;
  while ((y - left >= 0) && std::isdigit(data[x][y - left]))
  {
    ++left;
  }
  while ((y + right < data[x].size()) && std::isdigit(data[x][y + right]))
  {
    ++right;
  }

  return std::stoi(data[x].substr(y - left + 1, right + left - 1));
}

// find all the numbers around a '*' and return the product if there are exactly two
int get_gear_ratio(const std::vector<std::string> &data, size_t x, size_t y)
{
  char up = get_char(data, x - 1, y);
  char down = get_char(data, x + 1, y);
  char left = get_char(data, x, y - 1);
  char right = get_char(data, x, y + 1);
  char diag_up_right = get_char(data, x - 1, y + 1);
  char diag_up_left = get_char(data, x - 1, y - 1);
  char diag_down_right = get_char(data, x + 1, y + 1);
  char diag_down_left = get_char(data, x + 1, y - 1);

  std::vector<int> numbers;

  // get any numbers above
  if (std::isdigit(diag_up_left) || std::isdigit(diag_up_right))
  {
    if (std::isdigit(up))
    {
      // there is a number above, doesn't matter if it's all three places
      numbers.push_back(get_number(data, x - 1, y));
    }
    else
    {
      if (std::isdigit(diag_up_left))
      {
        numbers.push_back(get_number(data, x - 1, y - 1));
      }
      if (std::isdigit(diag_up_right))
      {
        numbers.push_back(get_number(data, x - 1, y + 1));
      }
    }
  }
  else if (std::isdigit(up))
  {
    // there's a single number, it's directly up
    numbers.push_back(get_number(data, x - 1, y));
  }

  if (std::isdigit(left))
  {
    numbers.push_back(get_number(data, x, y - 1));
  }
  if (std::isdigit(right))
  {
    numbers.push_back(get_number(data, x, y + 1));
  }

  if (std::isdigit(diag_down_left) || std::isdigit(diag_down_right))
  {
    if (std::isdigit(down))
    {
      // there is a number below, doesn't matter if it's all three places
      numbers.push_back(get_number(data, x + 1, y));
    }
    else
    {
      if (std::isdigit(diag_down_left))
      {
        numbers.push_back(get_number(data, x + 1, y - 1));
      }
      if (std::isdigit(diag_down_right))
      {
        numbers.push_back(get_number(data, x + 1, y + 1));
      }
    }
  }
  else if (std::isdigit(down))
  {
    // there's a single number, it's directly down
    numbers.push_back(get_number(data, x + 1, y));
  }

  return numbers.size() == 2 ? numbers[0] * numbers[1] : 0;
}

// find '*' and compute the gear ratio
int gear_ratio(const std::vector<std::string> &data)
{
  int sum = 0;
  for (size_t i = 0; i < data.size(); ++i)
  {
    for (size_t j = 0; j < data[i].size(); ++j)
    {
      if (data[i][j] == '*')
      {
        sum += get_gear_ratio(data, i, j);
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

  int part1_sum = count_part1(data);
  int part2_gear_ratio = gear_ratio(data);

  std::cout << "Part 1: " << part1_sum << std::endl;
  std::cout << "Part 1: " << part2_gear_ratio << std::endl;
}