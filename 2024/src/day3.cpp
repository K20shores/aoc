#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <regex>

struct Data {
  std::vector<std::string> lines;
};

int part1(const Data &data)
{
  int acc = 0;

  std::regex mul("mul\\(([0-9]+),([0-9]+)\\)");
  for(auto& line : data.lines) {
    auto mul_begin = std::sregex_iterator(line.begin(), line.end(), mul);
    auto mul_end = std::sregex_iterator();

    for (std::sregex_iterator i = mul_begin; i != mul_end; ++i)
    {
        std::smatch match = *i;
        std::string match_str = match.str();
        int left = std::stoi(match[1]);
        int right = std::stoi(match[2]);
        acc += left * right;
    }
  }
  return acc;
}

int part2(const Data &data)
{
  int acc = 0;
  int control = 1;

  std::regex mul("mul\\(([0-9]+),([0-9]+)\\)|(don\\'t\\(\\)|do\\(\\))");
  for(auto& line : data.lines) {
    auto mul_begin = std::sregex_iterator(line.begin(), line.end(), mul);
    auto mul_end = std::sregex_iterator();

    for (std::sregex_iterator i = mul_begin; i != mul_end; ++i)
    {
        std::smatch match = *i;
        std::string match_str = match.str();
        if (match_str == "do()") {
          control = 1;
        }
        else if (match_str == "don't()") {
          control = 0;
        }
        else {
          int left = std::stoi(match[1]);
          int right = std::stoi(match[2]);
          acc += left * right * control;
        }
    }
  }
  return acc;

}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day3.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    data.lines.push_back(line);
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

  int answer1 = 188192787;
  int answer2 = 113965544;

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