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
  std::vector<std::array<int, 4>> robots;
};

void iterate(std::vector<std::array<int, 4>> &robots, int t, int width, int height)
{
  for (auto &[px, py, vx, vy] : robots)
  {
    px = (t * vx + px) % width;
    if (px < 0)
    {
      px += width;
    }
    py = (t * vy + py) % height;
    if (py < 0)
    {
      py += height;
    }
  }
}

int64_t safety(std::vector<std::array<int, 4>> &robots, int width, int height)
{
  int64_t q1 = 0, q2 = 0, q3 = 0, q4 = 0;

  for (auto &[px, py, vx, vy] : robots)
  {
    if (px < width / 2 && py < height / 2)
    {
      q1 += 1;
    }
    else if (px > width / 2 && py < height / 2)
    {
      q2 += 1;
    }
    else if (px < width / 2 && py > height / 2)
    {
      q3 += 1;
    }
    else if (px > width / 2 && py > height / 2)
    {
      q4 += 1;
    }
  }
  return q1 * q2 * q3 * q4;
}

int64_t part1(const Data &data)
{
  int t = 100;
  int width = 101;
  int height = 103;
  auto robots = data.robots;

  iterate(robots, t, width, height);
  return safety(robots, width, height);
}

int part2(const Data &data)
{
  int width = 101;
  int height = 103;
  auto robots = data.robots;
  std::vector<int64_t> danger;
  int64_t min_danger = INT_MAX;
  int64_t min_t = INT_MAX;

  for (int t = 0; t < 10000; ++t)
  {
    iterate(robots, 1, width, height);
    danger.push_back(safety(robots, width, height));
    if (danger.back() < min_danger)
    {
      min_danger = danger.back();
      min_t = t;
    }
  }
  robots = data.robots;
  iterate(robots, min_t + 1, width, height);
  std::vector<std::string> grid(height, std::string(width, '.'));
  for (auto &[px, py, _, __] : robots)
  {
    grid[py][px] = '#';
  }
  for (auto &row : grid)
  {
    std::cout << row << std::endl;
  }
  std::cout << std::endl;

  return min_t + 1;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day14.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    int p1, p2, v1, v2;
    if (sscanf(line.c_str(), "p=%d,%d v=%d,%d", &p1, &p2, &v1, &v2) == 4)
    {
      data.robots.push_back({p1, p2, v1, v2});
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
    int64_t s = part1(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, Part2Benchmark)
(benchmark::State &state)
{
  for (auto _ : state)
  {
    int64_t s = part2(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark)->Unit(benchmark::kMillisecond);

int main(int argc, char **argv)
{
  Data data = parse();

  int64_t answer1 = 225552000;
  int64_t answer2 = 7371;

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