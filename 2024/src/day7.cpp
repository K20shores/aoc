#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <aoc/2024/split.h>
#include <functional>
#include <cmath>

struct Data {
  std::vector<long long> targets;
  std::vector<std::vector<int>> nums;
};

using Ops = std::vector<std::function<long long(long long, long long)>>;

// set default opts
Ops default_ops = {
  std::plus<long long>(),
  std::multiplies<long long>()
};

std::function<long long(long long, long long)> concat = [](long long a, long long b) {
  long long number = b;
  int digits = 0;
  if (number < 0) digits = 1;
  while (number) {
      number /= 10;
      digits++;
  }
  return a * std::pow(10, digits) + b;
};

bool is_possible(long long target, long long acc, const std::vector<int>& nums, size_t idx = 0, const Ops& ops = default_ops) {
  if (idx == nums.size()) {
    return acc == target;
  }

  for (auto op : ops) {
    if (is_possible(target, op(acc, nums[idx]), nums, idx + 1, ops)) {
      return true;
    }
  }
  return false;
}

long long part1(const Data &data)
{
  long long sum = 0;
  for(size_t i = 0; i < data.targets.size(); ++i) {
    long long target = data.targets[i];
    std::vector<int> nums = data.nums[i];

    if (is_possible(target, 0, nums)) {
      sum += target;
    }
  }
  return sum;
}

long long part2(const Data &data)
{
  long long sum = 0;
  auto ops = default_ops;
  ops.push_back(concat);
  for(size_t i = 0; i < data.targets.size(); ++i) {
    long long target = data.targets[i];
    std::vector<int> nums = data.nums[i];

    if (is_possible(target, 0, nums, 0, ops)) {
      sum += target;
    }
  }
  return sum;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day7.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    auto pos = line.find(':');
    data.targets.push_back(std::stoll(line.substr(0, pos)));
    data.nums.push_back(split_to_int(line.substr(pos+2), " "));
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

  long long answer1 = 10741443549536;
  long long answer2 = 500335179214836;

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
