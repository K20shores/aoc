#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <queue>

struct Data {
  std::vector<std::vector<long>> sequences;
};

int part1(Data data)
{
  int sum = 0;
  for(auto& sequence : data.sequences) {
    for(int i = sequence.size() - 1; i >= 0; --i){
      for(size_t j = 0; j < i; ++j) {
        sequence[j] = sequence[j+1] - sequence[j];
      }
      size_t l = 1;
      long diff = sequence[0];
      while(diff == sequence[l] && l < i){
        ++l;
      }
      if (l == i) {
        for(size_t k = l - 1; k < sequence.size(); ++k) {
          sum += sequence[k];
        }
        break;
      }
    }
  }
  return sum;
}

int part2(Data &data)
{
  int sum = 0;
  for(auto& sequence : data.sequences) {
    std::reverse(sequence.begin(), sequence.end());
    for(int i = sequence.size() - 1; i >= 0; --i){
      for(size_t j = 0; j < i; ++j) {
        sequence[j] = sequence[j+1] - sequence[j];
      }
      size_t l = 1;
      long diff = sequence[0];
      while(diff == sequence[l] && l < i){
        ++l;
      }
      if (l == i) {
        for(size_t k = l - 1; k < sequence.size(); ++k) {
          sum += sequence[k];
        }
        break;
      }
    }
  }
  return sum;
}

std::vector<long> parse_numbers(const std::string &line)
{
  std::vector<long> numbers;
  size_t num_start = -1;
  bool isneg = false;
  for (size_t i = 0; i <= line.size(); ++i)
  {
    if (std::isdigit(line[i]))
    {
      if (num_start == -1)
      {
        num_start = i;
      }
    }
    else if (line[i] == '-') {
      isneg = true;
    }
    else if (num_start != -1)
    {
      numbers.push_back((isneg ? -1 : 1) *std::stol(line.substr(num_start, i - num_start)));
      num_start = -1;
      isneg = false;
    }
  }
  return numbers;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day9.txt"));
  std::string line;
  Data data;
  
  while (std::getline(file, line))
  {
    data.sequences.push_back(parse_numbers(line));
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

  std::cout << "Part 1: " << part1(data) << std::endl;
  std::cout << "Part 2: " << part2(data) << std::endl;

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}