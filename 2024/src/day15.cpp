#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <benchmark/benchmark.h>
#include <format>
#include <aoc/2024/pos.h>
#include <queue>

struct Data
{
  std::vector<std::string> grid;
  std::vector<Pos> directions;
  Pos robot;
};

bool in_bounds(const std::vector<std::string> &grid, const Pos &p)
{
  return p.x >= 0 && p.y >= 0 && p.x < grid[0].size() && p.y < grid.size();
};

int64_t score(std::vector<std::string> &grid)
{
  int64_t score = 0;
  for (size_t j = 0; j < grid.size(); ++j)
  {
    for (size_t i = 0; i < grid[j].size(); ++i)
    {
      if (grid[j][i] == 'O' || grid[j][i] == '[')
      {
        score += 100 * j + i;
      }
    }
  }
  return score;
}

Pos find_free_space(const std::vector<std::string> &grid, const Pos &start, const Pos &dir)
{
  Pos open_space = start + dir;
  while (in_bounds(grid, open_space) && grid[open_space.y][open_space.x] != '#')
  {
    if (grid[open_space.y][open_space.x] == '.')
    {
      return open_space;
    }
    open_space += dir;
  }
  return {.x = -1, .y = -1};
}

int64_t part1(const Data &data)
{
  auto grid = data.grid;
  Pos cur = data.robot;
  for (auto dir : data.directions)
  {
    Pos next = cur + dir;
    if (in_bounds(grid, next))
    {
      char c = grid[next.y][next.x];
      if (c == '.')
      {
        grid[next.y][next.x] = grid[cur.y][cur.x];
        grid[cur.y][cur.x] = '.';
        cur = next;
      }
      else if (c == 'O')
      {
        Pos open_space = find_free_space(grid, next, dir);

        if (open_space.x != -1)
        {
          while (open_space != next)
          {
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

std::vector<std::string> transform_grid(const std::vector<std::string> &grid)
{
  std::vector<std::string> new_grid;
  for (auto &row : grid)
  {
    std::string new_row = "";
    for (auto &c : row)
    {
      if (c == '.')
      {
        new_row += "..";
      }
      else if (c == '#')
      {
        new_row += "##";
      }
      else if (c == 'O')
      {
        new_row += "[]";
      }
      else if (c == '@')
      {
        new_row += "@.";
      }
    }
    new_grid.push_back(new_row);
  }
  return new_grid;
}

void print_grid(const std::vector<std::string> &grid)
{
  for (auto &row : grid)
  {
    std::cout << row << std::endl;
  }
  std::cout << std::endl;
}

int64_t part2(const Data &data)
{
  auto grid = transform_grid(data.grid);
  Pos cur = {.x = data.robot.x * 2, .y = data.robot.y};
  for (auto dir : data.directions)
  {
    Pos next = cur + dir;
    if (in_bounds(grid, next))
    {
      char c = grid[next.y][next.x];
      if (c == '.')
      {
        grid[next.y][next.x] = grid[cur.y][cur.x];
        grid[cur.y][cur.x] = '.';
        cur = next;
      }
      else if (c == '[' || c == ']')
      {
        // open space, ending space
        std::vector<std::pair<Pos, Pos>> open_spaces;
        std::queue<std::pair<Pos, Pos>> q;
        q.push({next, next});

        if (dir == N || dir == S)
        {
          if (c == '[')
          {
            q.push({next + E, next + E});
          }
          else
          {
            q.push({next + W, next + W});
          }
        }

        bool open = true;
        while (!q.empty())
        {
          auto [start, end] = q.front();
          q.pop();
          Pos open_space = start + dir;
          while (in_bounds(grid, open_space) && grid[open_space.y][open_space.x] != '#')
          {
            if (grid[open_space.y][open_space.x] == '.')
            {
              break;
            }
            if (grid[open_space.y][open_space.x] == '[' && (dir == N || dir == S))
            {
              q.push({open_space + E, open_space + E});
            }
            if (grid[open_space.y][open_space.x] == ']' && (dir == N || dir == S))
            {
              q.push({open_space + W, open_space + W});
            }
            open_space += dir;
          }
          if (in_bounds(grid, open_space) && grid[open_space.y][open_space.x] != '#')
          {
            start = open_space;
            open_spaces.push_back({start, end});
          }
          else
          {
            open = false;
            break;
          }
        }

        // sort first by y, then by x
        // and if I am going north, put the highest y first
        // if I am going north, put the lowest y first
        std::sort(open_spaces.begin(), open_spaces.end(), [&](const auto &a, const auto &b)
                  {
            if (a.second.y != b.second.y) {
              if (dir == N) {
                return a.second.y < b.second.y;
              }
              return a.second.y > b.second.y;
            }
            return a.second.x < b.second.x; });

        if (open)
        {
          for (auto &[open_space, end] : open_spaces)
          {
            while (open_space != end)
            {
              if (grid[open_space.y][open_space.x] == '.')
              {
                Pos before = open_space - dir;
                grid[open_space.y][open_space.x] = grid[before.y][before.x];
                grid[before.y][before.x] = '.';
              }
              open_space -= dir;
            }
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
    if (grid)
    {
      auto robot = line.find('@');
      if (robot != std::string::npos)
      {
        data.robot = {.x = static_cast<int64_t>(robot), .y = j};
      }
      data.grid.push_back(line);
    }
    else
    {
      for (auto c : line)
      {
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
  int64_t answer2 = 1509724;

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
