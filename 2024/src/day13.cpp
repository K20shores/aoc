#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <array>

struct Data
{
  std::vector<std::array<int, 6>> data;
};

int part1(const Data &data)
{
  for(auto& claw: data.data) {
    int a1 = claw[0];
    int a2 = claw[1];
    int b1 = claw[2];
    int b2 = claw[3];
    int prize1 = claw[4];
    int prize2 = claw[5];

    int m = 0;
    int n = 0;
  }
}

int part2(const Data &data)
{
  return 0;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day13.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    if (line.size() == 0)
    {
      continue;
    }
    auto A = line.find("A:");
    auto B = line.find("B:");
    auto Prize = line.find("Prize");
    auto X = line.find("X");
    auto Y = line.find("Y");
    auto comma = line.find(",");

    if (A != std::string::npos)
    {
      data.data.push_back({0, 0, 0, 0, 0, 0});
      data.data.back()[0] = std::stoi(line.substr(X + 2, comma - X - 2));
      data.data.back()[1] = std::stoi(line.substr(Y + 2, line.size() - Y - 2));
    }
    else if (B != std::string::npos)
    {
      data.data.back()[2] = std::stoi(line.substr(X + 2, comma - X - 2));
      data.data.back()[3] = std::stoi(line.substr(Y + 2, line.size() - Y - 2));
    }
    else if (Prize != std::string::npos)
    {
      data.data.back()[4] = std::stoi(line.substr(X + 2, comma - X - 2));
      data.data.back()[5] = std::stoi(line.substr(Y + 2, line.size() - Y - 2));
    }
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

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark)->Unit(benchmark::kMillisecond);

int main(int argc, char **argv)
{
  Data data = parse();

  int answer1 = 0;
  int answer2 = 0;

  auto first = part1(data);
  std::cout << "Part 1: " << first << std::endl;

  auto second = part2(data);
  std::cout << "Part 2: " << second << std::endl;

  first != answer1 ? throw std::runtime_error("Part 1 incorrect") : nullptr;
  second != answer2 ? throw std::runtime_error("Part 2 incorrect") : nullptr;

  for (int i = 1; i < argc; ++i)
  {
    if (std::string(argv[i]) == "--benchmark")
    {
      benchmark::Initialize(&argc, argv);
      benchmark::RunSpecifiedBenchmarks();
      return 0;
    }
  }
}