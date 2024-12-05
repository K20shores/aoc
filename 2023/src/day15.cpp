#include <benchmark/benchmark.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Data {
  std::vector<std::string> sequence;
};

int hash(const std::string &s) {
  int hash = 0;
  for (const auto &c : s) {
    hash += c;
    hash *= 17;
    hash %= 256;
  }
  return hash;
}

int part1(const Data &data) {
  int sum = 0;
  for (const auto &line : data.sequence) {
    sum += hash(line);
  }
  return sum;
}

int part2(const Data &data) {
  std::vector<std::vector<std::pair<std::string, int>>> boxes(256);
  for (const auto &line : data.sequence) {
    size_t i = 0;
    while (line[i] != '=' && line[i] != '-') ++i;
    std::string label = line.substr(0, i);
    int loc = hash(label);
    char operation = line[i];

    auto &box = boxes[loc];
    auto it = std::find_if(box.begin(), box.end(), [&label](const auto &p) { return label == p.first; });
    switch (operation) {
      case '=': {
        int focal = std::stoi(line.substr(++i));
        if (it != box.end()) {
          it->second = focal;
        } else {
          box.push_back({label, focal});
        }
      } break;
      case '-':
        if (it != box.end()) {
          box.erase(it);
        }
        break;
    }
  }
  int sum = 0;
  for (size_t box = 0; box < boxes.size(); ++box) {
    size_t idx = 0;
    for (const auto &contents : boxes[box]) {
      sum += (1 + box) * (1 + idx) * contents.second;
      ++idx;
    }
  }
  return sum;
}

Data parse() {
  std::ifstream file(std::filesystem::path("inputs/day15.txt"));
  std::string line;
  Data data;

  size_t idx = 0;
  while (std::getline(file, line)) {
    for (size_t j = 0; j <= line.size(); ++j) {
      if (line[j] == ',' || line[j] == '\0') {
        data.sequence.push_back(line.substr(idx, j - idx));
        idx = j + 1;
        ++j;
      }
    }
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

  int answer1 = 503487;
  int answer2 = 261505;

  auto first = part1(data);
  auto second = part2(data);

  std::cout << "Part 1: " << first << std::endl;
  std::cout << "Part 2: " << second << std::endl;

  first != answer1 ? throw std::runtime_error("Part 1 incorrect") : nullptr;
  second != answer2 ? throw std::runtime_error("Part 2 incorrect") : nullptr;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}