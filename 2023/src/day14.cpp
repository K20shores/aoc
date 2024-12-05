#include <aoc/2023/hash.hpp>
#include <benchmark/benchmark.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

struct Data {
  std::vector<std::string> rocks;
};

void north(Data &data) {
  for (size_t j = 0; j < data.rocks[0].size(); ++j) {
    for (size_t i = 0; i < data.rocks.size() - 1; ++i) {
      char c = data.rocks[i][j];
      if (c == '.') {
        for (size_t k = i + 1; k < data.rocks.size(); ++k) {
          char s = data.rocks[k][j];
          if (s == 'O') {
            data.rocks[i][j] = 'O';
            data.rocks[k][j] = '.';
            ++i;
          } else if (s == '#') {
            break;
          }
        }
      }
    }
  }
}

void south(Data &data) {
  for (size_t j = 0; j < data.rocks[0].size(); ++j) {
    for (int i = data.rocks.size() - 1; i >= 0; --i) {
      char c = data.rocks[i][j];
      if (c == '.') {
        for (int k = i - 1; k >= 0; --k) {
          char s = data.rocks[k][j];
          if (s == 'O') {
            data.rocks[i][j] = 'O';
            data.rocks[k][j] = '.';
            --i;
          } else if (s == '#') {
            break;
          }
        }
      }
    }
  }
}

void west(Data &data) {
  for (int i = 0; i < data.rocks.size(); ++i) {
    for (size_t j = 0; j < data.rocks[0].size(); ++j) {
      char c = data.rocks[i][j];
      if (c == '.') {
        for (int k = j + 1; k < data.rocks[0].size(); ++k) {
          char s = data.rocks[i][k];
          if (s == 'O') {
            data.rocks[i][j] = 'O';
            data.rocks[i][k] = '.';
            ++j;
          } else if (s == '#') {
            break;
          }
        }
      }
    }
  }
}

void east(Data &data) {
  for (int i = 0; i < data.rocks.size(); ++i) {
    for (int j = data.rocks[0].size() - 1; j >= 0; --j) {
      char c = data.rocks[i][j];
      if (c == '.') {
        for (int k = j - 1; k >= 0; --k) {
          char s = data.rocks[i][k];
          if (s == 'O') {
            data.rocks[i][j] = 'O';
            data.rocks[i][k] = '.';
            --j;
          } else if (s == '#') {
            break;
          }
        }
      }
    }
  }
}

int count(const Data &data) {
  int sum = 0;
  for (size_t i = 0; i < data.rocks.size(); ++i) {
    for (size_t j = 0; j < data.rocks[0].size(); ++j) {
      if (data.rocks[i][j] == 'O') {
        sum += data.rocks.size() - i;
      }
    }
  }
  return sum;
}

int part1(Data data) {
  north(data);
  return count(data);
}

int part2(Data &data) {
  std::vector<std::pair<size_t, int>> hashes;
  int sum = 0;
  size_t cycles = 1000000000;
  for (size_t i = 0; i < cycles; ++i) {
    north(data);
    west(data);
    south(data);
    east(data);
    auto val = hash(data.rocks);
    auto it = std::find_if(hashes.begin(), hashes.end(), [&val](const std::pair<size_t, int> &p) { return p.first == val; });
    if (it != hashes.end()) {
      auto tail = std::distance(hashes.begin(), it);
      auto cycle = hashes.size() - tail;
      auto final = (cycles - i) % cycle;
      std::advance(it, final - 1);
      return it->second;
    } else {
      hashes.push_back({val, count(data)});
    }
  }
}

Data parse() {
  std::ifstream file(std::filesystem::path("inputs/day14.txt"));
  std::string line;
  Data data;

  while (std::getline(file, line)) {
    data.rocks.push_back(line);
  }

  return data;
}

class BenchmarkFixture : public benchmark::Fixture {
public:
  static Data data;
};

Data BenchmarkFixture::data = parse();

BENCHMARK_DEFINE_F(BenchmarkFixture, Part1Benchmark)
(benchmark::State &state) {
  for (auto _ : state) {
    int s = part1(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, Part2Benchmark)
(benchmark::State &state) {
  for (auto _ : state) {
    int s = part2(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark);

int main(int argc, char **argv) {
  Data data = parse();

  int answer1 = 109755;
  int answer2 = 90928;

  auto first = part1(data);
  auto second = part2(data);

  std::cout << "Part 1: " << first << std::endl;
  std::cout << "Part 2: " << second << std::endl;

  first != answer1 ? throw std::runtime_error("Part 1 incorrect") : nullptr;
  second != answer2 ? throw std::runtime_error("Part 2 incorrect") : nullptr;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}