#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>

struct Data
{
  std::vector<std::string> rocks;
};

int part1(Data &data)
{
  int sum = 0;
  for (size_t j = 0; j < data.rocks[0].size(); ++j)
  {
    for (size_t i = 0; i < data.rocks.size() - 1; ++i)
    {
      char c = data.rocks[i][j];
      if (c == '.')
      {
        for (size_t k = i+1; k < data.rocks.size(); ++k)
        {
          char s = data.rocks[k][j];
          if (s == 'O') {
            data.rocks[i][j] = 'O';
            data.rocks[k][j] = '.';
            ++i;
          }
          else if (s == '#') {
            break;
          }
        }
      }
    }
  }

  for(size_t i = 0; i < data.rocks.size(); ++i) {
    for(size_t j = 0; j < data.rocks[0].size(); ++j) {
      if(data.rocks[i][j] == 'O') {
        sum += data.rocks.size() - i;
      }
    }
  }


  for (auto &line : data.rocks)
  {
    std::cout << line << std::endl;
  }
  std::cout << std::endl;
  return sum;
}

int part2(const Data &data)
{
  return 0;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day14.txt"));
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    data.rocks.push_back(line);
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

  int answer1 = 1;
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