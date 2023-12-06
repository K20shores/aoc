#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <numeric>
#include <benchmark/benchmark.h>

struct Range
{
  long src;
  long dest;
  long range;
};

struct Data
{
  std::vector<long> seeds;
  std::vector<std::vector<Range>> ranges;
};

long part1(const Data &data)
{
  long min = LONG_MAX;
  std::vector<long> seed_locations = data.seeds;
  for (const auto &ranges : data.ranges)
  {
    for (size_t seed = 0; seed < data.seeds.size(); ++seed)
    {
      long location = seed_locations[seed];
      for (const auto &range : ranges)
      {
        if (location >= range.src && location < range.src + range.range)
        {
          location = (location - range.src) + range.dest;
          break;
        }
      }
      seed_locations[seed] = location;
    }
  }
  for (auto &loc : seed_locations)
  {
    min = std::min(min, loc);
  }
  return min;
}

int part2(const Data &data)
{
  int sum = 0;
  return sum;
}

std::vector<long> parse_numbers(const std::string &line)
{
  std::vector<long> numbers;
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
      numbers.push_back(std::stol(line.substr(num_start, i - num_start)));
      num_start = -1;
    }
  }
  return numbers;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day5.txt"));
  std::string line;
  Data data;

  std::vector<Range> ranges;
  while (std::getline(file, line))
  {
    if (line.starts_with("seeds:"))
    {
      data.seeds = parse_numbers(line);
    }
    else
    {
      if (line.find("-to-") != std::string::npos)
      {
        if (ranges.size() > 0)
        {
          data.ranges.push_back(ranges);
          ranges.clear();
        }
      }
      else if (std::isdigit(line[0]))
      {
        auto numbers = parse_numbers(line);
        ranges.push_back({.src = numbers[1], .dest = numbers[0], .range = numbers[2]});
      }
    }
  }
  if (ranges.size() > 0)
  {
    data.ranges.push_back(ranges);
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
    int sum = part1(data);
    benchmark::DoNotOptimize(sum);
  }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, Part2Benchmark)
(benchmark::State &state)
{
  for (auto _ : state)
  {
    int sum = part2(data);
    benchmark::DoNotOptimize(sum);
  }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark);

int main(int argc, char **argv)
{
  Data data = parse();

  std::cout << "Part 1: " << part1(data) << std::endl;
  std::cout << "Part 2: " << part2(data) << std::endl;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}