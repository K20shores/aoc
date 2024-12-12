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

struct Data {
  std::vector<std::string> gardens;
};

struct Node {
  Node* up;
  Node* down;
  Node* left;
  Node* right;
  char value;
};

int part1(const Data &data)
{
  std::set<Pos> visited;
  std::map<char, std::vector<std::pair<int, int>>> dimensions;
  // std::vector<std::unique_ptr<Node>> roots;
  std::queue<Pos> q;
  q.push(Pos(0, 0));

  char c = '\0';
  int area = 0;
  int perimeter = 0;

  auto in_bounds = [&](const Pos& p) {
    return p.x >= 0 && p.y >= 0 && p.x < data.gardens[0].size() && p.y < data.gardens.size();
  };

  while(q.size() > 0) {
    Pos p = q.front();
    visited.insert(p);

    if (data.gardens[p.y][p.x] != c) {
      if (area != 0 && perimeter != 0 && c != '\0') {
        dimensions[c].push_back({area, perimeter});
      }
      area = 0;
      perimeter = 0;
      c = data.gardens[p.y][p.x];
    }
    area += 1;
    for(auto dir: directions) {
      Pos next = p + dir;
      if (in_bounds(next)) {
        if (data.gardens[next.y][next.x] != c) {
          ++perimeter;
        }
        if (!visited.contains(next)) {
          q.push(p + dir);
        }
      }
    }
    q.pop();
  }

  for(auto& [key, dims] : dimensions) {
    std::cout << key << ": " << dims.size() << std::endl;
    // for(auto& dim : dims) {
    //   std::cout << std::format("Area: {}, Perimeter: {} | ", dim.first, dim.second);
    // }
    // std::cout << std::endl;
  }

  return 0;
}

int part2(const Data &data)
{
  return 0;
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

  int answer1 = 0;
  int answer2 = 0;

  auto first = part1(data);
  std::cout << "Part 1: " << first << std::endl;
  
  auto second = part2(data);
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