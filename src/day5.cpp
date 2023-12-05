#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <numeric>
#include <benchmark/benchmark.h>

int part1() {
  int sum = 0;
  return sum;
}

int part2() {
  int sum = 0;
  return sum;
}

std::vector<std::pair<std::vector<int>, std::vector<int>>> parse()
{
  std::ifstream file(std::filesystem::path("inputs/day5.txt"));
  std::string line;
  std::vector<std::pair<std::vector<int>, std::vector<int>>> data;

  while (std::getline(file, line))
  {
  }
  return data;
}

class BenchmarkFixture : public benchmark::Fixture {
public:
    void SetUp(const benchmark::State& state) override {
        data = parse();
    }

    void TearDown(const benchmark::State&) override {
        // Clean up if needed
        data.clear();
    }

    std::vector<std::pair<std::vector<int>, std::vector<int>>> data;
};

BENCHMARK_DEFINE_F(BenchmarkFixture, Part1Benchmark)(benchmark::State& state) {
    for (auto _ : state) {
        int sum = part1();
        benchmark::DoNotOptimize(sum);
    }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, Part2Benchmark)(benchmark::State& state) {
    for (auto _ : state) {
        int sum = part2();
        benchmark::DoNotOptimize(sum);
    }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark);

int main(int argc, char** argv) {
  std::vector<std::pair<std::vector<int>, std::vector<int>>> data = parse();

  int part1_sum = part1();
  int part2_sum = part2();

  std::cout << "Part 1: " << part1_sum << std::endl;
  std::cout << "Part 2: " << part2_sum << std::endl;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}