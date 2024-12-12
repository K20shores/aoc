#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <aoc/2024/split.h>
#include <stack>
#include <queue>

struct Data
{
  std::vector<int> stones;
};

uint64_t count_stones(int blinks, const Data &data)
{
  std::map<uint64_t, uint64_t> numbers;
  for (auto &stone : data.stones)
  {
    numbers[stone] = 1;
  }

  for (size_t i = 0; i < blinks; ++i)
  {
    std::map<uint64_t, uint64_t> new_numbers;
    for (const auto &[value, count] : numbers)
    {
      uint64_t num_digits = std::floor(std::log10(value)) + 1;
      if (value == 0)
      {
        new_numbers[1] += count;
      }
      else if (num_digits % 2 == 0)
      {
        uint64_t right_half = value % uint64_t(std::pow(10, num_digits / 2));
        uint64_t left_half = value / std::pow(10, num_digits / 2);
        new_numbers[left_half] += count;
        new_numbers[right_half] += count;
      }
      else
      {
        new_numbers[value * 2024] += count;
      }
    }
    numbers.swap(new_numbers);
  }

  uint64_t sum = 0;

  for (auto &[value, count] : numbers)
  {
    sum += count;
  }

  return sum;
}

uint64_t part1(const Data &data)
{
  return count_stones(25, data);
}

uint64_t part2(const Data &data)
{
  return count_stones(75, data);
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day11.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    data.stones = split_to_int(line, " ");
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
    uint64_t s = part1(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, Part2Benchmark)
(benchmark::State &state)
{
  for (auto _ : state)
  {
    uint64_t s = part2(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark)->Unit(benchmark::kMillisecond);

int main(int argc, char **argv)
{
  Data data = parse();

  uint64_t answer1 = 198089;
  uint64_t answer2 = 236302670835517;

  auto first = part1(data);
  std::cout << "Part 1: " << first << std::endl;

  auto second = part2(data);
  std::cout << "Part 2: " << second << std::endl;

  first != answer1 ? throw std::runtime_error("Part 1 incorrect") : nullptr;
  second != answer2 ? throw std::runtime_error("Part 2 incorrect") : nullptr;

  for (int i = 1; i < argc; ++i)
  {
    if (std::string(argv[i]) == "--benchmark")
    {
      benchmark::Initialize(&argc, argv);
      benchmark::RunSpecifiedBenchmarks();
      return 0;
    }
  }
}
