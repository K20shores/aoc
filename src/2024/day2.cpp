#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <iterator>

struct Data {
  std::vector<std::vector<int>> reports;
};

bool is_monotonic(const std::vector<int>& report) {
  auto left = report.begin();
  auto right = std::next(left, 1);
  bool increasing = false;
  bool decreasing = false;
  while (left != report.end() && right != report.end()) {
    bool _increasing = (*left < *right) && (*right > *left);
    bool _decreasing = (*left > *right) && (*right < *left);
    increasing |= _increasing;
    decreasing |= _decreasing;

    if (increasing && decreasing) return false;

    if (_increasing || _decreasing)
    {
      int diff = std::abs(*left - *right);
      if (!(diff >= 1 && diff <= 3)) {
        return false;
      } 
    }
    else {
      return false;
    }
    left = std::next(left, 1);
    right = std::next(right, 1);
  }
  return true;
}

int part1(const Data &data)
{
  int safe = 0;
  for(auto& report: data.reports) {
    if (is_monotonic(report)) {
      safe += 1;
    }
  }
  return safe;
}

int part2(const Data &data)
{
  int safe = 0;
  for(auto& report: data.reports) {
    if (is_monotonic(report)) {
      safe += 1;
    }
    else {
      for (size_t i = 0; i < report.size(); ++i) {
        std::vector<int> copy(report);
        auto it = std::next(copy.begin(), i);
        copy.erase(it);
        if (is_monotonic(copy)) {
          safe += 1;
          break;
        }
      }
    }
  }
  return safe;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day2.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    std::istringstream buffer(line);
    std::vector<int> report{std::istream_iterator<int>(buffer),
                            std::istream_iterator<int>()};
    data.reports.push_back(report);
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

  int answer1 = 402;
  int answer2 = 0;

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