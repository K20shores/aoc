#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <map>
#include <aoc/2024/pos.h>
#include <format>
#include <set>

struct Data
{
  size_t width;
  size_t height;
  std::map<char, std::vector<Pos>> locations;
  std::vector<std::string> map;
};

int part1(const Data &data)
{
  std::set<Pos> unique_locations;

  for (const auto &[key, positions] : data.locations)
  {
    for(size_t i = 0; i < positions.size() - 1; ++i) {
      Pos left = positions[i];
      for(size_t j = i+1; j < positions.size(); ++j) {
        Pos right = positions[j];
        Pos dif = left - right;
        Pos p1 = right - dif;
        Pos p2 = left + dif;
        for(const auto& p : {p1, p2}) {
          if (0 <= p.x && p.x < data.width && 0 <= p.y && p.y < data.height) {
            unique_locations.insert(p);
          }
        }
      }
    }
  }

  return unique_locations.size();;
}

int part2(const Data &data)
{
  return 0;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day8.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;
  data.height = 0;

  int64_t row = 0;
  while (std::getline(file, line))
  {
    data.map.push_back(line);
    data.width = line.size();
    for (int64_t col = 0; col < line.size(); ++col)
    {
      char c = line[col];
      if (c != '.')
      {
        if (data.locations.find(c) == data.locations.end())
        {
          data.locations[c] = {};
        }
        data.locations[c].push_back({.x = row, .y = col});
      }
    }
    row += 1;
  }
  data.height = row;

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

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark)->Unit(benchmark::kSecond);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark)->Unit(benchmark::kSecond);

int main(int argc, char **argv)
{
  Data data = parse();

  int answer1 = 271;
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