#include <aoc2023/maths.hpp>
#include <benchmark/benchmark.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Data {
  std::vector<std::string> record;
  std::vector<std::vector<int>> broken_spirngs;
};

int arrangements(const std::string line, const std::vector<int> &springs) {
  if (line.size() == 0 && springs.size() == 0) {
    return 1;
  } else if (line.size() == 0 && springs.size() == 0) {
    return 0;
  }

  char c = line[0];
  switch (c) {
    case '.': {
      return arrangements(line.substr(1), springs);
    }
    case '#': {
      if (springs[0] <= line.size()) {
        for (size_t j = 0; j < springs[0]; ++j) {
          if (line[j] == '.') {
            return 0;
          }
        }
        if (springs[0] < line.size() && line[springs[0]] == '#') {
          return 0;
        }
        auto s = springs;
        s.erase(s.begin());
        return arrangements(line.substr(springs[0]), s);
      } else {
        return 0;
      }
    }
    case '?': {
      std::string dot = line, hash = line;
      dot[0] = '.';
      hash[0] = '#';
      return arrangements(dot, springs) + arrangements(hash, springs);
      break;
    }
  }
}

int part1(const Data &data) {
  int sum = 0;
  auto record_it = data.record.begin();
  auto springs_it = data.broken_spirngs.begin();
  while (record_it != data.record.end()) {
    sum += arrangements(*record_it, *springs_it);
    ++record_it;
    ++springs_it;
  }
  return sum;
}

int part2(const Data &data) { return 0; }

std::vector<int> parse_numbers(const std::string &line) {
  std::vector<int> numbers;
  size_t num_start = -1;
  for (size_t i = 0; i <= line.size(); ++i) {
    if (std::isdigit(line[i])) {
      if (num_start == -1) {
        num_start = i;
      }
    } else if (num_start != -1) {
      numbers.push_back(std::stoi(line.substr(num_start, i - num_start)));
      num_start = -1;
    }
  }
  return numbers;
}

Data parse() {
  std::ifstream file(std::filesystem::path("inputs/day12.txt"));
  std::string line;
  Data data;

  while (std::getline(file, line)) {
    auto it = line.find(' ');
    data.record.push_back(line.substr(0, it));
    data.broken_spirngs.push_back(parse_numbers(line.substr(it + 1)));
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

  int answer1 = 2;
  int answer2 = 0;

  auto first = part1(data);
  auto second = part2(data);

  std::cout << "Part 1: " << first << std::endl;
  std::cout << "Part 2: " << second << std::endl;

  first != answer1 ? throw std::runtime_error("Part 1 incorrect") : nullptr;
  second != answer2 ? throw std::runtime_error("Part 2 incorrect") : nullptr;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}