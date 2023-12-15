#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

struct Cubes
{
  size_t red = 0;
  size_t green = 0;
  size_t blue = 0;
};

struct Game
{
  size_t id;
  std::vector<Cubes> draws;

  Game(std::string line)
  {
    auto space = line.find_first_of(' ');
    auto colon = line.find(':');
    id = std::stoi(line.substr(space + 1, colon - space - 1));
    line = line.substr(colon + 2);

    size_t num_start = -1;
    size_t red = 0;
    size_t blue = 0;
    size_t green = 0;
    size_t num = 0;

    for (size_t i = 0; i < line.size(); ++i)
    {
      char c = line[i];
      if ('0' <= c && c <= '9')
      {
        if (num_start == -1)
        {
          num_start = i;
        }
      }
      else
      {
        if (num_start != -1) {
          num = std::stoi(line.substr(num_start, i - num_start));
          num_start = -1;
        }
        else {
          if (c == 'g') {
            green = num;
            i += 4;
          }
          if (c == 'r') {
            red = num;
            i += 2;
          }
          if (c == 'b') {
            blue = num;
            i += 3;
          }
          if (c == ';') {
            draws.push_back({.red = red, .green = green, .blue = blue});
            red = 0;
            green = 0;
            blue = 0;
          }
        }
      }
    }
    draws.push_back({.red = red, .green = green, .blue = blue});
  }
};

int part1(const Game& g, size_t red_cubes = 12, size_t green_cubes = 13, size_t blue_cubes = 14)
{
  for(const auto& draw : g.draws) {
    if (draw.red > red_cubes || draw.blue > blue_cubes || draw.green > green_cubes) {
      return 0;
    }
  }
  return g.id;
}

int part2(const Game& g)
{
  size_t _red = 0;
  size_t _blue = 0;
  size_t _green = 0;
  for(const auto& draw : g.draws) {
    if (draw.blue != 0) {
      _blue = std::max(draw.blue, _blue);
    }
    if (draw.red != 0) {
      _red = std::max(draw.red, _red);
    }
    if (draw.green != 0) {
      _green = std::max(draw.green, _green);
    }
  }
  return _red * _blue * _green;
}

int main()
{
  std::ifstream file(std::filesystem::path("inputs/day2.txt"));
  std::string line;
  int sum_part1 = 0;
  int sum_part2 = 0;
  std::vector<Game> games;
  while (std::getline(file, line))
  {
    games.push_back(Game(line));
  }

  for(const auto& g : games) {
    sum_part1 += part1(g);
    sum_part2 += part2(g);
  }
  std::cout << "Part 1: " << sum_part1 << std::endl;
  std::cout << "Part 2: " << sum_part2 << std::endl;
}