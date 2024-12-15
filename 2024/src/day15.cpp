#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <aoc/2024/pos.h>

struct Data {
  std::vector<std::string> grid;
  std::vector<Pos> directions;
  Pos robot;
};

bool in_bounds(const Data &data, const Pos &p)
{
  return p.x >= 0 && p.y >= 0 && p.x < data.grid[0].size() && p.y < data.grid.size();
};

int64_t score(std::vector<std::string> &grid)
{
  int64_t score = 0;
  for(size_t j = 0; j < grid.size(); ++j) {
    for(size_t i = 0; i < grid[j].size(); ++i) {
      if (grid[j][i] == 'O') {
        score += 100 * j + i;
      }
    }
  }
  return score;
}

int64_t part1(const Data &data)
{
  auto grid = data.grid;
  Pos cur = data.robot;
  for(auto dir : data.directions) {
    Pos next = cur + dir;
    if (in_bounds(data, next)) {
      char c = grid[next.y][next.x];
      if (c == '.') {
        grid[next.y][next.x] = grid[cur.y][cur.x];
        grid[cur.y][cur.x] = '.';
        cur = next;
      }
      else if (c == 'O') {
        Pos open_space = next + dir;
        bool open = false;
        while (in_bounds(data, open_space) && grid[open_space.y][open_space.x] != '#') {
          if (grid[open_space.y][open_space.x] == '.') {
            open = true;
            break;
          }
          open_space += dir;
        }

        if (open) {
          while(open_space != next) {
            Pos before = open_space - dir;
            grid[open_space.y][open_space.x] = grid[before.y][before.x];
            grid[before.y][before.x] = '.';
            open_space -= dir;
          }

          grid[next.y][next.x] = grid[cur.y][cur.x];
          grid[cur.y][cur.x] = '.';
          cur = next;
        }
      }
    }
  }

  return score(grid);
}

int64_t part2(const Data &data)
{
  return 0;
}

Data parse()
{
  std::ifstream file(std::filesystem::path("inputs/day15.txt"));
  if (!file.is_open())
  {
    throw std::runtime_error("file not found");
  }
  std::string line;
  Data data;

  bool grid = true;
  int64_t j = 0;
  while (std::getline(file, line))
  {
    if (line.empty())
    {
      grid = false;
      continue;
    }
    if (grid) {
      auto robot = line.find('@');
      if (robot != std::string::npos) {
        data.robot = {.x = static_cast<int64_t>(robot), .y = j};
      }
      data.grid.push_back(line);
    }
    else {
      for(auto c : line) {
        switch (c)
        {
          case '^':
            data.directions.push_back(N);
            break;
          case 'v':
            data.directions.push_back(S);
            break;
          case '<':
            data.directions.push_back(W);
            break;
          case '>':
            data.directions.push_back(E);
            break;
        }
      }
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

  int64_t answer1 = 1475249;
  int64_t answer2 = 0;

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