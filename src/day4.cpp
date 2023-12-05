#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <numeric>
#include <benchmark/benchmark.h>

int part1(const std::vector<std::pair<std::vector<int>, std::vector<int>>>& data) {
  int sum = 0;
  for(const auto& pair : data){
    auto winning_nums = pair.first.begin();
    auto player_nums = pair.second.begin();
    int matches = 0;
    while (winning_nums != pair.first.end() && player_nums != pair.second.end()) {
      if (*winning_nums == *player_nums) {
        ++matches;
        ++winning_nums;
        ++player_nums;
        continue;
      }
      if (*winning_nums < *player_nums) {
        ++winning_nums;
      }
      else if (*player_nums < *winning_nums) {
        ++player_nums;
      }
    }
    sum += std::pow(2, matches-1) * (matches > 0);
  }
  return sum;
}

int part2(const std::vector<std::pair<std::vector<int>, std::vector<int>>>& data) {
  std::vector<int> copies(data.size(), 1);
  for(size_t i = 0; i < data.size(); ++i){
    const auto& pair = data[i];
    auto winning_nums = pair.first.begin();
    auto player_nums = pair.second.begin();
    int matches = 0;
    while (winning_nums != pair.first.end() && player_nums != pair.second.end()) {
      if (*winning_nums == *player_nums) {
        ++matches;
        ++winning_nums;
        ++player_nums;
        continue;
      }
      if (*winning_nums < *player_nums) {
        ++winning_nums;
      }
      else if (*player_nums < *winning_nums) {
        ++player_nums;
      }
    }
    for(size_t j = i+1; j <= i+matches; ++j) {
      copies[j] += 1 * copies[i];
    }
  }
  return std::accumulate(copies.begin(), copies.end(), 0);
}

std::vector<std::pair<std::vector<int>, std::vector<int>>> parse()
{
  std::ifstream file(std::filesystem::path("inputs/day4.txt"));
  std::string line;
  std::vector<std::pair<std::vector<int>, std::vector<int>>> data;

  while (std::getline(file, line))
  {
    std::pair<std::vector<int>, std::vector<int>> row;
    bool winning = true, collect = false;
    size_t length = 0;
    size_t num = 0;
    for(size_t i = 0; i <= line.size(); ++i) {
      char c = line[i];
      if (c == '|') {
        winning = false;
      }
      else if (c == ':') {
        collect = true;
      }
      else if (collect && std::isdigit(c)) {
        num = num * std::pow(10, length) + (c - '0');
        ++length;
      }
      else if (num != 0) {
        if (winning) {
          row.first.push_back(num);
        }
        else {
          row.second.push_back(num);
        }
        length = 0;
        num = 0;
      }
    }
    std::sort(row.first.begin(), row.first.end());
    std::sort(row.second.begin(), row.second.end());
    data.push_back(row);
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
        int sum = part1(data);
        benchmark::DoNotOptimize(sum);
    }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, Part2Benchmark)(benchmark::State& state) {
    for (auto _ : state) {
        int sum = part2(data);
        benchmark::DoNotOptimize(sum);
    }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark);

int main(int argc, char** argv) {
  std::vector<std::pair<std::vector<int>, std::vector<int>>> data = parse();

  int part1_sum = part1(data);
  int part2_sum = part2(data);

  std::cout << "Part 1: " << part1_sum << std::endl;
  std::cout << "Part 2: " << part2_sum << std::endl;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}