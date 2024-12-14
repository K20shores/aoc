#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <aoc/2024/pos.h>
#include <map>
#include <set>
#include <queue>

struct Data
{
  std::vector<std::string> gardens;
};

bool in_bounds(const Data &data, const Pos &p)
{
  return p.x >= 0 && p.y >= 0 && p.x < data.gardens[0].size() && p.y < data.gardens.size();
};

struct Dimensions {
  int area;
  int perimeter;
  int sides;
};

int count_corners(const Data &data, Pos cur)
{
  int cornes = 0;
  auto same = [&](Pos next){ return (in_bounds(data, next) && data.gardens[cur.y][cur.x] == data.gardens[next.y][next.x]);};

  bool n = same(cur+N);
  bool ne = same(cur+NE);
  bool e = same(cur+E);
  bool se = same(cur+SE);
  bool s = same(cur+S);
  bool sw = same(cur+SW);
  bool w = same(cur+W);
  bool nw = same(cur+NW);

  // convex corners, or an L shaped region where a cell is the same as two of it's orthogonal neighbors 
  // AX XA  A  A
  //  A A  AX  AX 
  // concave corners, or when a cell is the same as its two orthongal neighbors but not the same
  // as its diagonal neighboer
  // AA AA AX XA
  // XA XA AA AA

  // the left side of the || operator checks convex corners
  // the right side of the || operators chekcs concave corners
  if (!n && !e || n && e && !ne) cornes += 1;
  if (!e && !s || e && s && !se) cornes += 1;
  if (!s && !w || s && w && !sw) cornes += 1;
  if (!w && !n || w && n && !nw) cornes += 1;

  return cornes;
}


std::map<char, std::vector<Dimensions>> floodfill(const Data& data) {
  std::set<Pos> unvisited;
  std::map<char, std::vector<Dimensions>> dimensions;

  for (int64_t j = 0; j < data.gardens.size(); ++j)
  {
    for (int64_t i = 0; i < data.gardens[0].size(); ++i)
    {
      unvisited.insert({.x = i, .y = j});
    }
  }

  while (!unvisited.empty())
  {
    Pos p = *unvisited.begin();
    std::queue<Pos> q;

    char target = data.gardens[p.y][p.x];
    Dimensions d{.area = 0, .perimeter = 0, .sides = 0};
    q.push(p);

    // floodfill
    while (!q.empty())
    {
      Pos cur = q.front();
      if (!in_bounds(data, cur) || data.gardens[cur.y][cur.x] != target) {
        ++d.perimeter;
      }
      else if (unvisited.contains(cur)) {
        unvisited.erase(cur);
        ++d.area;
        d.sides += count_corners(data, cur);
        for (auto dir : directions)
        {
          q.push(cur + dir);
        }
      }
      q.pop();
    }

    dimensions[target].push_back(d);
  }

  return dimensions;
}

int part1(const Data &data)
{
  auto dimensions = floodfill(data);
  int sum = 0;
  for (auto &[key, val] : dimensions)
  {
    for (auto &d : val)
    {
      sum += d.area * d.perimeter;
    }
  }
  return sum;
}

int part2(const Data &data)
{
  auto dimensions = floodfill(data);
  int sum = 0;
  for (auto &[key, val] : dimensions)
  {
    for (auto &d : val)
    {
      sum += d.area * d.sides;
    }
  }
  return sum;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day12.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  while (std::getline(file, line))
  {
    data.gardens.push_back(line);
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

  int answer1 = 1396298;
  int answer2 = 853588;

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