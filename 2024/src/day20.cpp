#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <aoc/2024/pos.h>

struct Data
{
  Pos start;
  Pos end;
  std::vector<std::string> grid;
};

bool in_bounds(const Pos &pos, int width, int height)
{
  return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}

std::vector<Pos> djikstra(const Data &data, int64_t width, int64_t height)
{
  std::priority_queue<std::pair<int64_t, Pos>, std::vector<std::pair<int64_t, Pos>>, std::greater<>> pq;
  std::map<Pos, int64_t> dist;
  std::map<Pos, std::vector<Pos>> prev;
  std::set<Pos> visited;

  Pos start = data.start;
  pq.emplace(0, start);
  dist[start] = 0;

  int64_t distance = 0;
  while (!pq.empty())
  {
    auto elem = pq.top();
    pq.pop();
    int64_t cur_dist = elem.first;
    Pos u = elem.second;
    visited.insert(u);

    if (u == data.end)
    {
      distance = cur_dist;
      break;
    }

    for (auto &new_dir : directions)
    {
      Pos v = u + new_dir;

      if (!in_bounds(v, width, height) || visited.contains(v) || data.grid[v.y][v.x] == '#')
      {
        continue;
      }

      int64_t new_dist = cur_dist + 1;
      if (!dist.contains(v) || new_dist < dist[v])
      {
        dist[v] = new_dist;
        prev[v] = {u};
        pq.emplace(new_dist, v);
      }
      else if (new_dist == dist[v])
      {
        prev[v].push_back(u);
      }
    }
  }

  std::vector<Pos> path;
  Pos cur = data.end;
  while (cur != data.start)
  {
    path.push_back(cur);
    auto &prevs = prev[cur];
    cur = prevs[0];
  }
  path.push_back(data.start);

  return path;
}

int count_time_saving_cheats(const Data &data, int cheat_size, int min_savings)
{
  auto result = djikstra(data, data.grid[0].size(), data.grid.size());
  int sum = 0;
  for(int i = 0; i < result.size(); ++i) {
    Pos p1 = result[result.size() - 1 - i];
    for(int j = i + 1; j < result.size(); ++j) {
      Pos p2 = result[result.size() - 1 - j];
      int dist = std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
      int savings = j - i - dist;
      if (dist <= cheat_size && savings >= min_savings) {
        ++sum;
      }
    }
  }
  return sum;
}

int part1(const Data &data)
{
  return count_time_saving_cheats(data, 2, 100);
}

int part2(const Data &data)
{
  return count_time_saving_cheats(data, 20, 100);
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day20.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  int64_t j = 0;
  while (std::getline(file, line))
  {
    data.grid.push_back(line);
    auto E = line.find("E");
    auto S = line.find("S");
    if (E != std::string::npos)
    {
      data.end = Pos{.x = static_cast<int64_t>(E), .y = j};
    }
    if (S != std::string::npos)
    {
      data.start = Pos{.x = static_cast<int64_t>(S), .y = j};
    }
    ++j;
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
    auto s = part1(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_DEFINE_F(BenchmarkFixture, Part2Benchmark)
(benchmark::State &state)
{
  for (auto _ : state)
  {
    auto s = part2(data);
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_REGISTER_F(BenchmarkFixture, Part1Benchmark)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(BenchmarkFixture, Part2Benchmark)->Unit(benchmark::kMillisecond);

int main(int argc, char **argv)
{
  Data data = parse();

  int answer1 = 1311;
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
