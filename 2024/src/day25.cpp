#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>

struct Data {
  std::vector<std::vector<int>> keys;
  std::vector<std::vector<int>> locks;
};

void sort_values(std::vector<std::vector<int>>& values, bool reverse = false) {
  std::sort(values.begin(), values.end(), [&](const auto& a, const auto& b) {
    for(int i = 0; i < a.size(); ++i) {
      if (a[i] != b[i]) {
        if (reverse) {
          return a[i] > b[i];
        }
        return a[i] < b[i];
      }
    }
    return false;
  });
}

int part1(const Data &data)
{
  auto keys = data.keys;
  auto locks = data.locks;
  sort_values(keys);
  sort_values(locks);
  for(auto& key : keys) {
    std::cout << std::format("key: {} {} {} {} {}\n", key[0], key[1], key[2], key[3], key[4]);
  }
  std::cout << std::format("-----\n");
  for(auto& lock : locks) {
    std::cout << std::format("lock: {} {} {} {} {}\n", lock[0], lock[1], lock[2], lock[3], lock[4]);
  }
  std::cout << std::format("-----\n");
  std::cout << std::format("Number of keys: {}\n", keys.size());
  std::cout << std::format("Number of locks: {}\n", locks.size());

  int sum = 0;
  for(auto key: keys) {
    for(auto lock: locks) {
      bool valid = true;
      for(int i = 0; i < key.size(); ++i) {
        if (key[i] + lock[i] > 5) {
          valid = false;
          break;
        }
      }
      if (valid) {
        // std::cout << "key: ";
        // for(int i = 0; i < key.size(); ++i) {
        //   std::cout << std::format("{} ", key[i]);
        // }
        // std::cout << "lock: ";
        // for(int i = 0; i < lock.size(); ++i) {
        //   std::cout << std::format("{} ", lock[i]);
        // }
        // std::cout << " fit\n";
        sum++;
      }
    }
  }
  return sum;
}

int part2(const Data &data)
{
  return 0;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day25.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  bool locks = true;
  bool last_empty = true;
  std::vector<int> count(5, 0);
  while (std::getline(file, line))
  {
    if (line.size() == 0) {
      if (locks) {
        data.locks.push_back(count);
      }
      else {
        data.keys.push_back(count);
      }
      last_empty = true;
      count.assign(5, 0);
      continue;
    }
    if (line == "#####" && last_empty) {
      locks = true;
      last_empty = false;
      continue;
    }
    else if (line == "#####") {
      last_empty = false;
      continue;
    }
    if (line == "....." && last_empty) {
      locks = false;
      last_empty = false;
      continue;
    }
    last_empty = false;
    for(int i = 0; i < line.size(); ++i) {
      count[i] += line[i] == '#';
    }
  }
  if (locks) {
    data.locks.push_back(count);
  }
  else {
    data.keys.push_back(count);
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

  int answer1 = 0;
  int answer2 = 0;

  auto first = part1(data);
  std::cout << "Part 1: " << first << std::endl;
  
  auto second = part2(data);
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