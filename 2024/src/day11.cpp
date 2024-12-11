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

int part1(const Data &data)
{
  int sum = 0;
  std::map<int, std::vector<int>> splits;

  for (auto &stone : data.stones)
  {
    // stone value, depth
    std::queue<std::pair<int, int>> q;
    q.push({stone, 0});
    std::vector<int> split(25, 0);

    while (!q.empty())
    {
      auto stone = q.front();
      if (!splits.contains(stone.first)) {
        splits[stone.first] = std::vector(25, 0);
      }

      for (size_t blink = stone.second; blink < 25; ++blink)
      {
        if (splits.contains(stone.first))
        {
          for(size_t i = blink; i < splits[stone.first].size(); ++i) {
            split[i] += splits[stone.first][i];
          }
          blink = splits[stone.first].size();
          continue;
        }

        int num_digits = std::floor(std::log10(stone.first)) + 1;
        if (stone.first == 0)
        {
          stone.first = 1;
        }
        else if (num_digits % 2 == 0)
        {
          int right_half = stone.first % int(std::pow(10, num_digits / 2));
          stone.first /= std::pow(10, num_digits / 2);
          q.push({right_half, stone.second + 1});
          split[blink] += 1;
        }
        else
        {
          stone.first *= 2024;
        }
        stone.second += 1;
      }
      q.pop();
    }

    splits[stone] = split;
    sum += split[24];
  }
    int j = 0;
    j +=  1;

  return 0;
}

int part2(const Data &data)
{
  return 0;
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
  std::cout << "Parsed " << data.stones.size() << " stones" << std::endl;

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

  int answer1 = 0;
  int answer2 = 0;

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
