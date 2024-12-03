#include <benchmark/benchmark.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

struct Mapping {
  int64_t src;
  int64_t dest;
  int64_t length;
};

struct Data {
  std::vector<int64_t> seeds;
  std::vector<std::vector<Mapping>> ranges;
};

int64_t part1(const Data &data) {
  int64_t min = std::numeric_limits<int64_t>::max();
  for (const auto &seed : data.seeds) {
    int64_t location = seed;
    for (const auto &ranges : data.ranges) {
      for (const auto &range : ranges) {
        if (location >= range.src && location < range.src + range.length) {
          location = (location - range.src) + range.dest;
          break;
        }
        if (location < range.src) {
          break;
        }
      }
    }
    min = std::min(min, location);
  }
  return min;
}

int part2(const Data &data) {
  int64_t min = std::numeric_limits<int64_t>::max();
  for (size_t i = 0; i < data.seeds.size(); i += 2) {
    int64_t start = data.seeds[i];
    int64_t n = data.seeds[i + 1];
    for (int64_t seed = start; seed < start + n; ++seed) {
      int64_t location = seed;
      for (const auto &ranges : data.ranges) {
        for (const auto &range : ranges) {
          if (location >= range.src && location < range.src + range.length) {
            location = (location - range.src) + range.dest;
            break;
          }
          if (location < range.src) {
            break;
          }
        }
      }
      min = std::min(min, location);
    }
  }
  return min;
}

std::vector<int64_t> parse_numbers(const std::string &line) {
  std::vector<int64_t> numbers;
  size_t num_start = -1;
  for (size_t i = 0; i <= line.size(); ++i) {
    if (std::isdigit(line[i])) {
      if (num_start == -1) {
        num_start = i;
      }
    } else if (num_start != -1) {
      numbers.push_back(std::stoll(line.substr(num_start, i - num_start)));
      num_start = -1;
    }
  }
  return numbers;
}

bool compare_mappings(const Mapping &a, const Mapping &b) { return a.src < b.src; }

Data parse() {
  std::ifstream file(std::filesystem::path("inputs/day5.txt"));
  std::string line;
  Data data;

  std::vector<Mapping> ranges;
  while (std::getline(file, line)) {
    if (line.starts_with("seeds:")) {
      data.seeds = parse_numbers(line);
    } else {
      if (line.find("-to-") != std::string::npos) {
        if (ranges.size() > 0) {
          data.ranges.push_back(ranges);
          ranges.clear();
        }
      } else if (std::isdigit(line[0])) {
        auto numbers = parse_numbers(line);
        ranges.push_back({.src = numbers[1], .dest = numbers[0], .length = numbers[2]});
      }
    }
  }
  if (ranges.size() > 0) {
    data.ranges.push_back(ranges);
  }

  for (auto &range : data.ranges) {
    std::sort(range.begin(), range.end(), compare_mappings);
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

  std::cout << "Part 1: " << part1(data) << std::endl;
  std::cout << "Part 2: " << part2(data) << std::endl;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}