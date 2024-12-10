#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>

struct Data {
  std::vector<std::string> grid;
};

int check(const std::string& s) {
  if (s.size() == 4 && s == "XMAS") {
    return 1;
  }
  return 0;
}

int part1(const Data &data)
{
  size_t width = data.grid[0].size();
  size_t height = data.grid.size();
  int count = 0;

  for(size_t y = 0; y < data.grid.size(); ++y) {
    for(size_t x = 0; x < data.grid[0].size(); ++x) {
      // look forward
      if (width - x >= 4) {
        count += check(data.grid[y].substr(x, 4));
      }
      // look backward
      if (x >= 3) {
        std::string s = {data.grid[y][x], data.grid[y][x-1], data.grid[y][x-2], data.grid[y][x-3]};
        count += check(s);
      }
      // look along negative x diagonal forward
      if (height-y >= 4 && width-x >= 4) {
        std::string s = {data.grid[y][x], data.grid[y+1][x+1], data.grid[y+2][x+2], data.grid[y+3][x+3]};
        count += check(s);
      }
      // look along negative x diagonal backward
      if (y >= 3 && x >= 3) {
        std::string s = {data.grid[y][x], data.grid[y-1][x-1], data.grid[y-2][x-2], data.grid[y-3][x-3]};
        count += check(s);
      }
      // look along positive x diagonal forward
      if (y >= 3 && width - x >= 4) {
        std::string s = {data.grid[y][x], data.grid[y-1][x+1], data.grid[y-2][x+2], data.grid[y-3][x+3]};
        count += check(s);
      }
      // look along positive x diagonal backward
      if (height - y >= 4 && x >= 3) {
        std::string s = {data.grid[y][x], data.grid[y+1][x-1], data.grid[y+2][x-2], data.grid[y+3][x-3]};
        count += check(s);
      }
      // look up
      if (y >= 3) {
        std::string s = {data.grid[y][x], data.grid[y-1][x], data.grid[y-2][x], data.grid[y-3][x]};
        count += check(s);
      }
      // look down
      if (height - y >= 4) {
        std::string s = {data.grid[y][x], data.grid[y+1][x], data.grid[y+2][x], data.grid[y+3][x]};
        count += check(s);
      }
    }
  }
  return count;
}

int part2(const Data &data)
{
  size_t width = data.grid[0].size();
  size_t height = data.grid.size();
  int count = 0;

  for(size_t y = 1; y < data.grid.size() - 1; ++y) {
    for(size_t x = 1; x < data.grid[0].size() - 1; ++x) {
      if (data.grid[y][x] == 'A') {
        char top_left = data.grid[y-1][x-1];
        char top_right = data.grid[y-1][x+1];
        char bottom_left = data.grid[y+1][x-1];
        char bottom_right = data.grid[y+1][x+1];

        bool negative_diagonal_matches = (top_left == 'M' && bottom_right == 'S') || (top_left == 'S' && bottom_right == 'M');
        bool positive_diagonal_matches = (top_right == 'M' && bottom_left == 'S') || (top_right == 'S' && bottom_left == 'M');

        if (negative_diagonal_matches && positive_diagonal_matches) {
          count += 1;
        }
      }
    }
  }
  return count;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day4.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    data.grid.push_back(line);
  }

  return data;
}

class BenchmarkFixture : public benchmark::Fixture
{
public:
  static Data data;
};

Data BenchmarkFixture::data = parse();

BENCHMARK_DEFINE_F(BenchmarkFixture, Part1Benchmark)
(benchmark::State &state)
{
  for (auto _ : state)
  {
    int s = part1(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, Part2Benchmark)
(benchmark::State &state)
{
  for (auto _ : state)
  {
    int s = part2(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark)->Unit(benchmark::kMillisecond);

int main(int argc, char **argv)
{
  Data data = parse();

  int answer1 = 2654;
  int answer2 = 1990;

  auto first = part1(data);
  auto second = part2(data);

  std::cout << "Part 1: " << first << std::endl;
  std::cout << "Part 2: " << second << std::endl;

  first != answer1 ? throw std::runtime_error("Part 1 incorrect") : nullptr;
  second != answer2 ? throw std::runtime_error("Part 2 incorrect") : nullptr;

  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--benchmark") {
      benchmark::Initialize(&argc, argv);
      benchmark::RunSpecifiedBenchmarks();
      return 0;
    }
  }
}