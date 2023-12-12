#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <benchmark/benchmark.h>
#include <aoc2023/position.hpp>

struct Data
{
  std::vector<std::string> lines;
};

int part1(const Data &data)
{
  std::vector<size_t> empty_rows;
  std::vector<size_t> empty_columns;
  std::vector<Pos> positions;

  for (size_t i = 0; i < data.lines.size(); ++i)
  {
    bool found = false;
    for (size_t j = 0; j < data.lines[i].size(); ++j)
    {
      if (data.lines[i][j] == '#')
      {
        positions.push_back({i, j});
        found = true;
      }
    }
    if (!found)
    {
      empty_rows.push_back(i);
    }
  }
  for (size_t j = 0; j < data.lines[0].size(); ++j)
  {
    for (size_t i = 0; i < data.lines.size(); ++i)
    {
      if (data.lines[i][j] == '#')
        break;
      if (i == data.lines.size() - 1 && data.lines[i][j] == '.')
        empty_columns.push_back(j);
    }
  }

  for (auto &pos : positions)
  {
    size_t i = 0, j = 0;
    for (const auto &row : empty_rows)
    {
      if (row <= pos.i)
      {
        ++i;
      }
    }
    for (const auto &column : empty_columns)
    {
      if (column <= pos.j)
      {
        ++j;
      }
    }
    pos.i += i;
    pos.j += j;
  }

  int s = 0;
  for (size_t i = 0; i < positions.size() - 1; ++i)
  {
    auto p1 = positions[i];
    for (size_t j = i + 1; j < positions.size(); ++j)
    {
      auto p2 = positions[j];
      s += std::abs(int(p1.i - p2.i)) + std::abs(int(p1.j - p2.j));
    }
  }
  return s;
}

long part2(const Data &data)
{
  std::vector<size_t> empty_rows;
  std::vector<size_t> empty_columns;
  std::vector<Pos> positions;

  for (size_t i = 0; i < data.lines.size(); ++i)
  {
    bool found = false;
    for (size_t j = 0; j < data.lines[i].size(); ++j)
    {
      if (data.lines[i][j] == '#')
      {
        positions.push_back({i, j});
        found = true;
      }
    }
    if (!found)
    {
      empty_rows.push_back(i);
    }
  }
  for (size_t j = 0; j < data.lines[0].size(); ++j)
  {
    for (size_t i = 0; i < data.lines.size(); ++i)
    {
      if (data.lines[i][j] == '#')
        break;
      if (i == data.lines.size() - 1 && data.lines[i][j] == '.')
        empty_columns.push_back(j);
    }
  }

  for (auto &pos : positions)
  {
    size_t i = 0, j = 0;
    for (const auto &row : empty_rows)
    {
      if (row <= pos.i)
      {
        ++i;
      }
    }
    for (const auto &column : empty_columns)
    {
      if (column <= pos.j)
      {
        ++j;
      }
    }
    pos.i += (1e6-1) * i;
    pos.j += (1e6-1) * j;
  }

  long s = 0;
  for (size_t i = 0; i < positions.size() - 1; ++i)
  {
    auto p1 = positions[i];
    for (size_t j = i + 1; j < positions.size(); ++j)
    {
      auto p2 = positions[j];
      s += std::abs(long(p1.i - p2.i)) + std::abs(long(p1.j - p2.j));
    }
  }
  return s;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day11.txt"));
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    data.lines.push_back(line);
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

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark);

int main(int argc, char **argv)
{
  Data data = parse();

  int answer1 = 9521776;
  int answer2 = 0;

  auto first = part1(data);
  auto second = part2(data);

  std::cout << "Part 1: " << first << std::endl;
  std::cout << "Part 2: " << second << std::endl;

  first != answer1 ? throw std::runtime_error("Part 1 incorrect") : nullptr;
  second != answer2 ? throw std::runtime_error("Part 2 incorrect") : nullptr;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}