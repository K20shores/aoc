#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <aoc2023/position.hpp>

struct Data {
  std::vector<Pos> steps;
  std::vector<Pos> interiors;
  std::vector<std::string> colors;
};

int part1(const Data &data)
{
  int area = 0;
  int perim = 0;
  Pos prev = {0, 0};
  Pos iprev = {0, 0};
  std::vector<Pos> points;
  std::vector<Pos> interior_points;

  for(long i = 0; i < data.steps.size(); ++i) {
    Pos cur = {data.steps[i].i + prev.i, data.steps[i].j + prev.j};
    Pos icur = {data.interiors[i].i + iprev.i, data.interiors[i].j + iprev.j};
    points.push_back(cur);
    interior_points.push_back(icur);
    prev = cur;
    iprev = icur;
  }

  for(size_t i = 0; i < points.size(); ++i) {
    auto p1 = points[i];
    auto p2 = points[(i+1)%points.size()];
    int x = p2.i - p1.i;
    int y = p2.j - p1.j;
    perim += std::sqrt(x*x + y*y);
  }

  for(size_t i = 0; i < interior_points.size(); ++i) {
    auto p1 = interior_points[i];
    auto p2 = interior_points[(i+1)%interior_points.size()];
    int x = p2.i - p1.i;
    int y = p2.j - p1.j;
    area += (p1.i*p2.j - p2.i*p1.j);
  }
  area = abs(area)/2;
  // for(auto& p : points) std::cout << "(" << p.i << "," << p.j << "), ";
  return 0;
}

int part2(const Data &data)
{
  return 0;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day18.txt"));
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    char c = line[0];
    Pos dir;
    switch(c){
      case 'L': dir = {-1, 0}; break;
      case 'R': dir = {1, 0}; break;
      case 'D': dir = {0, -1}; break;
      case 'U': dir = {0, 1}; break;
    }
    size_t i = 2;
    while(line[i] != ' ') ++i;
    int l = std::stoi(line.substr(2, i - 2));
    i += 3;
    data.steps.push_back({dir.i * l, dir.j * l});
    ++l;
    data.interiors.push_back({dir.i * l, dir.j * l});
    data.colors.push_back(line.substr(i, 6));
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

  int answer1 = 0;
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