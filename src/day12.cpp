#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <aoc2023/maths.hpp>

struct Data
{
  std::vector<std::string> record;
  std::vector<std::vector<int>> broken_spirngs;
};

struct Count
{
  size_t broken = 0;
  size_t unknown = 0;

  void reset()
  {
    broken = 0;
    unknown = 0;
  }
};

int part1(const Data &data)
{
  int sum = 0;
  Count counts;
  for (size_t i = 0; i < data.record.size(); ++i)
  {
    size_t broken_idx = 0;
    int potential_springs = data.broken_spirngs[i][broken_idx];
    for (size_t j = 0; j < data.record[i].size(); ++j)
    {
      if (data.record[i][j] == '.') break;

      if (data.record[i][j] == '?') ++counts.unknown;

      if (potential_springs < counts.unknown) {
        if (broken_idx < data.broken_spirngs[i].size()-1){
          auto next = data.broken_spirngs[i][broken_idx+1];
          if (potential_springs + next < counts.unknown) {
            potential_springs += next;
            ++broken_idx;
          }
        }
      }

      if (data.record[i][j] == '#') ++counts.broken;
    }
    // n choose k - (n - k + 1)
    auto options = n_choose_k(counts.unknown, potential_springs) - counts.unknown - potential_springs + 1;
    std::cout << data.record[i] << " " << options << std::endl;
    int n = 0;
  }
  return 0;
}

int part2(const Data &data)
{
  return 0;
}

std::vector<int> parse_numbers(const std::string &line)
{
  std::vector<int> numbers;
  size_t num_start = -1;
  for (size_t i = 0; i <= line.size(); ++i)
  {
    if (std::isdigit(line[i]))
    {
      if (num_start == -1)
      {
        num_start = i;
      }
    }
    else if (num_start != -1)
    {
      numbers.push_back(std::stoi(line.substr(num_start, i - num_start)));
      num_start = -1;
    }
  }
  return numbers;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day12.txt"));
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    auto it = line.find(' ');
    data.record.push_back(line.substr(0, it));
    data.broken_spirngs.push_back(parse_numbers(line.substr(it + 1)));
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

  int answer1 = 2;
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