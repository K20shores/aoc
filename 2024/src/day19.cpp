#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <aoc/2024/split.h>
#include <aoc/2024/trie.h>

struct Data
{
  std::vector<std::string> patterns;
  std::vector<std::string> designs;
};

bool check_design(std::string_view design, const Trie &t, std::unordered_map<std::string_view, bool> &cache)
{
  if (design.empty())
  {
    return true;
  }

  if (cache.count(design))
  {
    return cache[design];
  }

  std::vector<int> match = t.search(design);
  for (auto m : match)
  {
    if (check_design(design.substr(m), t, cache))
    {
      cache[design] = true;
      return true;
    }
  }

  cache[design] = false;
  return false;
}

int64_t count_ways(std::string_view design, const Trie &t, std::unordered_map<std::string_view, int64_t> &cache)
{
  if (design.empty())
  {
    return 1;
  }

  if (cache.count(design))
  {
    return cache[design];
  }

  // Get all matching prefixes for the current design.
  std::vector<int> match = t.search(design);
  cache[design] = 0;
  for (auto m : match)
  {
    cache[design] += count_ways(design.substr(m), t, cache);
  }

  return cache[design];
}

int part1(const Data &data)
{
  Trie t;
  std::unordered_map<std::string_view, bool> cache;
  for (const auto &pattern : data.patterns)
  {
    t.insert(pattern);
  }

  int possible = 0;
  for (auto &design : data.designs)
  {
    if (check_design(design, t, cache))
    {
      ++possible;
    }
  }
  return possible;
}

int64_t part2(const Data &data)
{
  Trie t;
  std::unordered_map<std::string_view, int64_t> cache;
  for (const auto &pattern : data.patterns)
  {
    t.insert(pattern);
  }

  int64_t possible = 0;
  for (auto &design : data.designs)
  {
    auto ways = count_ways(design, t, cache);
    possible += ways;
    // std::cout << std::format("Design: {}, Ways: {}\n", design, ways);
  }
  return possible;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day19.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  bool patterns = true;
  while (std::getline(file, line))
  {
    if (line.size() == 0)
    {
      patterns = false;
      continue;
    }
    if (patterns)
    {
      data.patterns = split(line, ", ");
    }
    else
    {
      data.designs.push_back(line);
    }
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
    auto s = part1(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, Part2Benchmark)
(benchmark::State &state)
{
  for (auto _ : state)
  {
    auto s = part2(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark)->Unit(benchmark::kMillisecond);

int main(int argc, char **argv)
{
  Data data = parse();

  int answer1 = 367;
  int64_t answer2 = 724388733465031;

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
